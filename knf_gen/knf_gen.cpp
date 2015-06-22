#include <assert.h>
#include <vector>
#include <stdio.h>
#include <thread>

#include "producer.h"

#include "cryptominisat4/cryptominisat.h"
#include "module/const_32.h"
#include "module/bsig0_32.h"
#include "module/maj_32.h"
#include "module/adder_32.h"
#include "module/constadder_32.h"
#include "module/ssig0_32.h"
#include "module/adder_ssig_32.h"
#include "module/adder_prepare_32.h"

using std::vector;
using std::thread;
using namespace CMSat;

void calculate(Producer* producer) {
    vector<unsigned> v;
    while (producer->getWork(v)) {

        for (unsigned s = 0; s < (1u << v.size()); s++) {
            SATSolver solver;
            solver.log_to_file("solver.log");

            Adder_Ssig_32 adder;
            adder.append(&solver);

            vector<Lit> clause(1);
            for (unsigned i = 0; i < v.size(); i++) {
                clause[0] = Lit(v[i], (s >> i) & 1);
                solver.add_clause(clause);
            }

            lbool ret = solver.solve();
            if (ret == l_False) {
                std::cout << " Hurra: ";
                for (unsigned i = 0; i < v.size(); i++) {
                    std::cout << ((s >> i) & 1 ? "-" : "") << v[i] << " ";
                }
                std::cout << "\n";
            }
        }
    }
}

int main() {
    Adder_Ssig_32 adder;
    unsigned out = adder.getOutput();

    Producer producer(3);
    for (unsigned i = 0; i < 64; i++) producer.addVar(i);
    for (unsigned i = out; i < out + 32; i++) producer.addVar(i);

    thread first(calculate, &producer);
    thread second(calculate, &producer);
    thread third(calculate, &producer);
    thread fourth(calculate, &producer);

    first.join();
    second.join();
    third.join();
    fourth.join();
    
/*
    Adder_Ssig_32 adder;
    unsigned out = adder.getOutput();

    for (unsigned i1 = 0; i1 < 64; i1++) {
        for (unsigned i2 = i1 + 1; i2 < 64; i2++) {
            for (unsigned o = out; o < out + 32; o++) {
                for (unsigned s = 0; s < 8; s++) {
                    SATSolver solver;
                    solver.log_to_file("solver.log");

                    adder.append(&solver);
                    vector<Lit> clause(1);

                    clause[0] = Lit(i1, s & 1);
                    solver.add_clause(clause);

                    clause[0] = Lit(i2, (s >> 1) & 1);
                    solver.add_clause(clause);

                    clause[0] = Lit(o, (s >> 2) & 1);
                    solver.add_clause(clause);

                    lbool ret = solver.solve();
                    if (ret == l_False) {
                        std::cout << " Hurra: ";
                        std::cout << " i1: " << i1 << " s: " << !(s & 1);
                        std::cout << " i2: " << i2 << " s: " << !((s >> 1) & 1);
                        std::cout << "  o: " <<  o << " s: " << !((s >> 2) & 1) << "\n";
                    }

                    std::cout << "\r" << i1 * 16384 + i2 * 256 + (o - out) * 8 + s + 1 << " / " << 64 * 64 * 32 * 8;
                    std::cout << std::flush;
                }
            }
        }
    }
    std::cout << "\n";
*/
/*
    Adder_Prepare_32 ap;
    unsigned out = ap.getOutput();

    for (unsigned i1 = 0; i1 < 128; i1++) {
        for (unsigned i2 = 0; i2 < 128; i2++) {
            if (i1 == i2) continue;
            for (unsigned o = out; o < out + 32; o++) {
                for (unsigned s = 0; s < 8; s++) {
                    SATSolver solver;
                    solver.log_to_file("solver.log");

                    ap.append(&solver);
                    vector<Lit> clause(1);

                    clause[0] = Lit(i1, s & 1);
                    solver.add_clause(clause);

                    clause[0] = Lit(i2, (s >> 1) & 1);
                    solver.add_clause(clause);

                    clause[0] = Lit(o, (s >> 2) & 1);
                    solver.add_clause(clause);

                    lbool ret = solver.solve();
                    if (ret == l_False) std::cout << " Hurra\n";

                    std::cout << "\r" << i1 * 32768 + i2 * 256 + (o - out) * 8 + s + 1 << " / " << 128 * 128 * 32 * 8;
                    std::cout << std::flush;
                }
            }
        }
    }
    std::cout << "\n";
*/
/*
    SATSolver solver;
    solver.log_to_file("solver.log");
    solver.set_num_threads(4);

    Bsig0_32 bsig0;
    bsig0.writeDimacs("bsig0_32.dimacs");
    bsig0.writeTT("bsig0_32.tt");

    Maj_32 maj;
    maj.append(&solver);
    maj.writeDimacs("maj_32.dimacs");
    maj.writeTT("maj_32.tt");

    Adder_32 adder;
    adder.writeDimacs("adder_32.dimacs");
    adder.writeTT("adder_32.tt");

    ConstAdder_32 constadder(0xAAAAAAAA);
    constadder.writeDimacs("constadder_32.dimacs");
    constadder.writeTT("constadder_32.tt");

    Ssig0_32 ssig0;
    std::vector<unsigned> subinputs;
    subinputs.push_back(32);
    ssig0.setInputs(subinputs);
    ssig0.setStart(128);
    ssig0.writeDimacs("ssig0_32.dimacs");

    lbool ret = solver.solve();
    assert(ret == l_True);

    //for (unsigned i = 63; i >=32; i--) std::cout << (solver.get_model()[i] == l_True? 1 : 0); std::cout << std::endl;
*/
    return 0;
}
