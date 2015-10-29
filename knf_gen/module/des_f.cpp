#include "add_32.h"

#include "add_half_1.h"
#include "add_full_1.h"
#include "add_full_2.h"
#include "add_full_3.h"
#include "clausecreator.h"

#include "../common/solvertools.h"

using std::vector;
using namespace CMSat;

unsigned Des_F::stats[STATS_LENGTH];

Des_F::Des_F(uint round) : Modul(32, 2, 1) {
    setInputsBitWidth(32, 56);
    this->round = round;
    output = 136;
}

Des_F::~Des_F() {
}

unsigned* Des_F::getStats() {
    return stats;
}

void Des_F::create(Printer* printer) {
    printer->newModul(10, "Des_F", this);

    // E expansion
    int e_permutation[] = {32,  1,  2,  3,  4,  5,
                            4,  5,  6,  7,  8,  9,
                            8,  9, 10, 11, 12, 13,
                           12, 13, 14, 15, 16, 17,
                           16, 17, 18, 19, 20, 21,
                           20, 21, 22, 23, 24, 25,
                           24, 25, 26, 27, 28, 29,
                           28, 29, 30, 31, 32,  1};
    // K_1 ... generation
    int pc_2[] = {14, 17, 11, 24,  1,  5,  3, 28,
                  15,  6, 21, 10, 23, 19, 12,  4,
                  26,  8, 16,  7, 27, 20, 13,  2,
                  41, 52, 31, 37, 47, 55, 30, 40,
                  51, 45, 33, 48, 44, 49, 39, 56,
                  34, 53, 46, 42, 50, 36, 29, 32};

    std::vector<unsigned> k_rotated;
    for(int i = 0; i < 56; i++){
      k_rotated.append(inputs[1] + i);
    }
    std::vector<unsigned>::iterator c_begin = k_rotated.begin();
    std::vector<unsigned>::iterator c_end = k_rotated.begin()+27;
    std::vector<unsigned>::iterator d_begin = k_rotated.begin()+28;
    std::vector<unsigned>::iterator d_end = k_rotated.end();

    for(int i = 0; i < round; i++){
      if(i == 0 || i == 1 || i == 8 || i == 15){
        std::rotate(c_begin, c_begin+1, c_end);
        std::rotate(d_begin, d_begin+1, d_end);
      } else {
        std::rotate(c_begin, c_begin+2, c_end);
        std::rotate(d_begin, d_begin+2, d_end);
      }
    }

    for(int i = 0; i < 48; i++){
      createXOR(printer, start + i, inputs[0] + e_permutation[i] - 1, k_rotated[pc_2[i] - 1]);
    }
}

MU_TEST_C(Des_F::test) {
    unsigned a[] = {1234, 5, 0x80000000, 1, 0xFFFFFFFF, 0x2, 0xFFFFFFFF, 0x1, 0xFFFFFFFF, 0x0};
    unsigned b[] = {1235, 6, 1, 0x80000000, 0x2, 0xFFFFFFFF, 0x1, 0xFFFFFFFF, 0x0, 0xFFFFFFFF};

    for (unsigned t = 0; t < 10; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");

        uint32_t ausgabe = a[t] + b[t];

        solver_writeInt(solver,  0, 32, a[t]);
        solver_writeInt(solver, 32, 32, b[t]);

        Des_F adder;
        adder.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "Adder UNSAT");
        mu_assert(ausgabe == solver_readInt(solver, 95, 32), "Adder failed");
    }
}
