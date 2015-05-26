#include <cryptominisat4/cryptominisat.h>
#include <assert.h>
#include <vector>
#include <stdio.h>

#include "bsig0.h"

using std::vector;
using namespace CMSat;

int main()
{
    SATSolver solver;
    vector<Lit> clause;
    vector<unsigned> x_clause;

    solver.log_to_file("mylog.txt");

    //Let's use 4 threads
    solver.set_num_threads(4);

    //  BSIG0(x) = ROTR^2(x) XOR ROTR^13(x) XOR ROTR^22(x)

    // 32 Eingangsvariablen (0 bis 31) + 32 Ausgangsvariablen (32 bis 63)
    solver.new_vars(64);

    uint32_t a, b;
    a = 0xabcdef98;
    for (unsigned i = 32; i > 0; i--) {
        bool tmp = ((a >> (i - 1) & 1));
        std::cout << tmp;
        
        clause.clear();
        clause.push_back(Lit(i - 1, !tmp));
        solver.add_clause(clause);
    }
    std::cout << std::endl;

    b = (a >> 2 | a << (32-2)) ^ (a >> 13 | a << (32-13)) ^ (a >> 22 | a << (32-22));
    for (unsigned i = 32; i > 0; i--) std::cout << ((b >> (i - 1) & 1)); std::cout << std::endl;

    Lit l = Lit(2, true);
    printf("%d\n", l.toInt());

    Bsig0 bsig0;
    bsig0.append(&solver);

/*
    //adds "1 0"
    clause.push_back(Lit(0, false));
    solver.add_clause(clause);

    //adds "-2 0"
    clause.clear();
    clause.push_back(Lit(1, true));
    solver.add_clause(clause);

    //adds "-1 2 3 0"
    clause.clear();
    clause.push_back(Lit(0, true));
    clause.push_back(Lit(1, false));
    clause.push_back(Lit(2, false));
    solver.add_clause(clause);

    //xortest
    x_clause.clear();
    x_clause.push_back(3);
    x_clause.push_back(4);
    x_clause.push_back(5);
    solver.add_xor_clause(x_clause, true);
*/

    lbool ret = solver.solve();

    for (unsigned i = 63; i >=32; i--) std::cout << (solver.get_model()[i] == l_True? 1 : 0); std::cout << std::endl;
/*
    assert(ret == l_True);
    assert(solver.get_model()[0] == l_True);
    assert(solver.get_model()[1] == l_False);
    assert(solver.get_model()[2] == l_True);

    std::cout
    << "Solution is: "
    << solver.get_model()[0]
    << ", " << solver.get_model()[1]
    << ", " << solver.get_model()[2]
    << std::endl;
*/

    return 0;
}
