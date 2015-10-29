#include "des_sbox_7.h"

#include "clausecreator.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Des_SBox_7::stats[STATS_LENGTH];

Des_SBox_7::Des_SBox_7() : Modul(4, 1, 1) {
    setInputsBitWidth(6);
    output = 6;
}

Des_SBox_7::~Des_SBox_7() {
}

unsigned* Des_SBox_7::getStats() {
    return stats;
}

void Des_SBox_7::create(Printer* printer) {
    printer->newModul(0, "Des_SBox_7", this);

    ClauseCreator cc(printer);
    //                c_out  s_out[0]  s_out[1]    a_in[0]        a_in[1]    b_in[0]        b_in[1]       c_in
    cc.setLiterals(10, input[0]+0, input[0]+1, input[0]+2, input[0]+3, input[0]+4, input[0]+5, output+0, output+1, output+2, output+3);
    //cc.printClause(10, CC_DC,      0,      CC_DC,         1,         CC_DC,         1,         CC_DC,         1);
}

MU_TEST_C(Des_SBox_7::test) {
    unsigned in[]  = {0x25};
    unsigned out[] = {0x11};

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");

        solver_writeInt(solver, 0, 6, in[t]);

        Des_SBox_7 sbox_7;
        sbox_7.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "DES_SBOX_7 UNSAT");
        mu_assert(out[t] == solver_readInt(solver, 6, 4), "DES_SBOX_7 failed");
    }
}
