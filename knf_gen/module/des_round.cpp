#include "des_round.h"

#include  <algorithm>

#include "../common/solvertools.h"

using std::vector;
using namespace CMSat;

unsigned Des_Round::stats[STATS_LENGTH];

Des_F::Des_Round(uint round) : Modul(32, 3, 1) {
    setInputsBitWidth(32, 32, 56); //l, r, k
    this->round = round;
    output = 152;
}

Des_F::~Des_Round() {
}

unsigned* Des_F::getStats() {
    return stats;
}

void Des_F::create(Printer* printer) {
    printer->newModul(10, "Des_Round", this);

    // E expansion
    int p_permutation[] = {16,  7, 20, 21, 29, 12, 28, 17,
                           01, 15, 23, 26, 05, 18, 31, 10,
                           02,  8, 24, 14, 32, 27,  3,  9,
                           19, 13, 30,  6, 22, 11,  4, 25};

    Des_F f_function(round);
    f_function.setInputs(inputs[1], inputs[2]);
    f_function.setOutput(start);
    f_function.create(printer);

    for(int i = 0; i < 32; i++){
      createXOR(printer, output + i, inputs[0] + i, start + p_permutation[i] - 1);
    }
}

MU_TEST_C(Des_Round::test) {
    unsigned r[] = {1, 7, 5, 10, 1, 13};
    unsigned e[] = {0x00000000, 0x20f3f8c6, 0x4c908cc2, 0x18002a23, 0xfaf4268e, 0x826e4265};
    uint64_t k[] = {0x00000000000000, 0x5056c027267bbe, 0xa7134a2d8d43e9, 0xc98857f73219e8, 0xa20237622f24a3, 0xf727033f5d61c5};
    unsigned out[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");

        solver_writeInt_msb(solver,  0, 32, e[t]);
        solver_writeInt_msb(solver, 32, 56, k[t]);

        Des_F des_f_function(r[t]);
        des_f_function.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "DES_Round UNSAT");
/*
        printf("\n");
        printf("Erwartet: %08x\n", out[t]);
        printf("Erhalten: %08lx\n", solver_readInt_msb(solver, 136, 32));
*/
        mu_assert(out[t] == solver_readInt_msb(solver, 136, 32), "DES_Round failed");
    }
}
