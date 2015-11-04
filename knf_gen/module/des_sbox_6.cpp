#include "des_sbox_6.h"

#include "clausecreator.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Des_SBox_6::stats[STATS_LENGTH];

Des_SBox_6::Des_SBox_6() : Modul(4, 1, 1) {
    setInputsBitWidth(6);
    output = 6;
}

Des_SBox_6::~Des_SBox_6() {
}

unsigned* Des_SBox_6::getStats() {
    return stats;
}

void Des_SBox_6::create(Printer* printer) {
    printer->newModul(0, "Des_SBox_6", this);

    ClauseCreator cc(printer);
    cc.setLiterals(10, inputs[0]+0, inputs[0]+1, inputs[0]+2, inputs[0]+3, inputs[0]+4, inputs[0]+5, output+0, output+1, output+2, output+3);
    cc.printClause(10,     1, CC_DC,     0, CC_DC,     1, CC_DC, CC_DC,     0,     1,     1);
    cc.printClause(10, CC_DC,     1,     1, CC_DC, CC_DC,     0,     0,     1, CC_DC,     0);
    cc.printClause(10,     0, CC_DC, CC_DC,     0, CC_DC,     0, CC_DC,     0,     0,     1);
    cc.printClause(10, CC_DC, CC_DC,     1,     1,     0, CC_DC,     1,     0, CC_DC, CC_DC);
    cc.printClause(10,     0, CC_DC, CC_DC,     0, CC_DC,     0,     1, CC_DC,     1,     1);
    cc.printClause(10, CC_DC, CC_DC, CC_DC, CC_DC,     0,     1,     0,     0,     1,     1);
    cc.printClause(10,     1, CC_DC, CC_DC,     0,     1, CC_DC, CC_DC,     1,     1,     1);
    cc.printClause(10,     0,     1, CC_DC, CC_DC,     0, CC_DC, CC_DC,     0,     0,     0);
    cc.printClause(10,     0, CC_DC,     0, CC_DC,     1, CC_DC,     1, CC_DC,     1,     1);
    cc.printClause(10,     0,     0, CC_DC, CC_DC,     1,     1, CC_DC, CC_DC,     0,     1);
    cc.printClause(10, CC_DC,     1,     0,     1,     0, CC_DC, CC_DC, CC_DC,     0,     0);
    cc.printClause(10,     0, CC_DC,     0,     1,     1,     1,     0, CC_DC, CC_DC, CC_DC);
    cc.printClause(10,     0, CC_DC,     1,     0, CC_DC,     0,     0, CC_DC, CC_DC,     0);
    cc.printClause(10, CC_DC,     1,     1,     1,     0,     0, CC_DC, CC_DC,     1, CC_DC);
    cc.printClause(10, CC_DC, CC_DC, CC_DC,     0, CC_DC,     1,     0,     0,     0,     1);
    cc.printClause(10, CC_DC,     1,     0, CC_DC, CC_DC,     1, CC_DC,     0, CC_DC,     0);
    cc.printClause(10, CC_DC,     0, CC_DC,     0, CC_DC,     1, CC_DC,     0,     0,     0);
    cc.printClause(10,     1, CC_DC, CC_DC,     1, CC_DC, CC_DC,     0,     1,     1,     1);
    cc.printClause(10,     1, CC_DC,     0, CC_DC,     1,     1,     1,     1, CC_DC, CC_DC);
    cc.printClause(10, CC_DC,     1, CC_DC,     1,     1,     0,     1,     1, CC_DC, CC_DC);
    cc.printClause(10,     0,     1, CC_DC,     0,     1, CC_DC,     0,     1, CC_DC, CC_DC);
    cc.printClause(10, CC_DC, CC_DC,     0, CC_DC,     0, CC_DC, CC_DC,     1,     1,     0);
    cc.printClause(10, CC_DC,     0, CC_DC, CC_DC,     0, CC_DC,     1,     0,     1,     0);
    cc.printClause(10,     1, CC_DC, CC_DC, CC_DC,     1, CC_DC,     0, CC_DC,     0,     0);
    cc.printClause(10,     0,     1, CC_DC,     1, CC_DC,     1,     1, CC_DC, CC_DC,     0);
    cc.printClause(10,     0,     0, CC_DC, CC_DC, CC_DC, CC_DC,     1,     1,     0, CC_DC);
    cc.printClause(10,     1,     0,     0, CC_DC, CC_DC, CC_DC,     1,     0, CC_DC,     1);
    cc.printClause(10, CC_DC, CC_DC,     1,     0,     0, CC_DC, CC_DC,     1, CC_DC,     0);
    cc.printClause(10, CC_DC, CC_DC,     1,     1,     1,     1,     0, CC_DC,     0, CC_DC);
    cc.printClause(10, CC_DC, CC_DC, CC_DC, CC_DC,     1,     0,     0,     0, CC_DC,     1);
    cc.printClause(10, CC_DC,     0,     0, CC_DC, CC_DC,     0, CC_DC,     1,     1,     0);
    cc.printClause(10,     1,     1, CC_DC,     1,     0, CC_DC,     1,     0, CC_DC, CC_DC);
    cc.printClause(10,     1,     0,     0, CC_DC,     0, CC_DC,     1,     1, CC_DC, CC_DC);
    cc.printClause(10, CC_DC,     0, CC_DC,     1,     0,     1,     0, CC_DC,     0, CC_DC);
    cc.printClause(10,     0,     0,     1, CC_DC,     1,     1, CC_DC,     1, CC_DC, CC_DC);
    cc.printClause(10,     1,     0, CC_DC,     0, CC_DC,     0,     1,     0, CC_DC, CC_DC);
    cc.printClause(10,     1,     1, CC_DC,     0,     0,     1,     1, CC_DC, CC_DC, CC_DC);
    cc.printClause(10, CC_DC,     0,     1, CC_DC,     0,     0, CC_DC,     1,     0, CC_DC);
    cc.printClause(10,     0, CC_DC,     1, CC_DC,     1,     0, CC_DC,     0,     0, CC_DC);
    cc.printClause(10,     1,     0, CC_DC, CC_DC,     0,     1, CC_DC,     1,     1, CC_DC);
    cc.printClause(10,     1, CC_DC,     1,     0,     1,     0, CC_DC,     1, CC_DC, CC_DC);
    cc.printClause(10,     1, CC_DC,     1,     0,     1,     1, CC_DC, CC_DC,     1, CC_DC);
    cc.printClause(10,     0,     0,     0, CC_DC,     0,     1, CC_DC,     1, CC_DC, CC_DC);
    cc.printClause(10,     1,     0,     0, CC_DC,     0, CC_DC,     0,     0, CC_DC, CC_DC);
    cc.printClause(10,     0, CC_DC,     0,     1, CC_DC,     0,     0, CC_DC, CC_DC,     1);
    cc.printClause(10,     1,     1,     0,     0, CC_DC,     0, CC_DC, CC_DC, CC_DC,     1);
    cc.printClause(10,     1, CC_DC, CC_DC,     0,     0,     0, CC_DC, CC_DC,     0,     0);
    cc.printClause(10,     1,     0, CC_DC,     1,     1, CC_DC, CC_DC, CC_DC, CC_DC,     0);
    cc.printClause(10, CC_DC, CC_DC,     0, CC_DC,     1,     0, CC_DC,     0,     1, CC_DC);
    cc.printClause(10,     1, CC_DC,     1,     1, CC_DC,     1, CC_DC, CC_DC,     0, CC_DC);
    cc.printClause(10,     1,     1, CC_DC,     1,     0,     0, CC_DC,     1, CC_DC, CC_DC);
    cc.printClause(10,     0, CC_DC,     0,     0,     0, CC_DC, CC_DC, CC_DC,     1,     1);
    cc.printClause(10,     0,     0,     0,     0, CC_DC,     0, CC_DC, CC_DC,     0, CC_DC);
    cc.printClause(10,     1, CC_DC,     1,     1, CC_DC,     0, CC_DC, CC_DC,     1,     1);
    cc.printClause(10,     0,     1, CC_DC,     0,     0,     0, CC_DC, CC_DC, CC_DC,     0);
    cc.printClause(10,     1, CC_DC,     0,     1,     1,     0,     0, CC_DC, CC_DC, CC_DC);
    cc.printClause(10,     0,     0,     1,     1,     0, CC_DC, CC_DC, CC_DC, CC_DC,     0);
    cc.printClause(10, CC_DC,     0,     0,     1,     0,     0, CC_DC,     0, CC_DC, CC_DC);
    cc.printClause(10,     1,     0,     1,     1, CC_DC,     0,     0, CC_DC, CC_DC, CC_DC);
    cc.printClause(10,     0,     0,     1,     0,     0, CC_DC, CC_DC, CC_DC,     1, CC_DC);
    cc.printClause(10,     0,     0,     1,     1, CC_DC,     0, CC_DC, CC_DC,     1, CC_DC);
    cc.printClause(10,     1, CC_DC,     1,     0,     0,     0, CC_DC, CC_DC,     1, CC_DC);
    cc.printClause(10, CC_DC,     1, CC_DC, CC_DC, CC_DC, CC_DC,     0,     0,     1,     0);
    cc.printClause(10, CC_DC, CC_DC,     1, CC_DC, CC_DC,     1,     1, CC_DC,     0,     1);
    cc.printClause(10, CC_DC, CC_DC, CC_DC,     1, CC_DC,     1,     1,     0, CC_DC,     1);
    cc.printClause(10, CC_DC,     1, CC_DC, CC_DC, CC_DC,     1,     1, CC_DC,     1,     1);
    cc.printClause(10, CC_DC,     1, CC_DC,     0,     1, CC_DC,     1, CC_DC, CC_DC,     0);
    cc.printClause(10, CC_DC,     1,     0, CC_DC, CC_DC,     1,     0, CC_DC,     0, CC_DC);
    cc.printClause(10,     1,     0, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC,     1,     0,     1);
    cc.printClause(10, CC_DC,     1, CC_DC, CC_DC, CC_DC,     0, CC_DC,     0,     0,     1);
    cc.printClause(10, CC_DC, CC_DC, CC_DC,     0, CC_DC,     1,     0, CC_DC,     1,     0);
    cc.printClause(10, CC_DC,     1,     1, CC_DC,     0, CC_DC,     0,     1, CC_DC, CC_DC);
    cc.printClause(10, CC_DC,     0, CC_DC, CC_DC, CC_DC,     1,     0, CC_DC,     1,     1);
    cc.printClause(10,     0, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC,     0,     1,     0);
    cc.printClause(10,     0, CC_DC,     0,     1, CC_DC, CC_DC, CC_DC, CC_DC,     0,     0);
    cc.printClause(10,     0,     1, CC_DC, CC_DC,     0, CC_DC,     1, CC_DC, CC_DC,     1);
    cc.printClause(10,     0, CC_DC,     0,     0,     1, CC_DC,     1, CC_DC, CC_DC, CC_DC);
    cc.printClause(10,     0, CC_DC,     1, CC_DC,     1, CC_DC,     0, CC_DC, CC_DC,     1);
    cc.printClause(10,     1, CC_DC,     1,     1,     1, CC_DC, CC_DC, CC_DC, CC_DC,     0);
}

MU_TEST_C(Des_SBox_6::test) {
    unsigned in[]  = {0x25};
    unsigned out[] = {0x4};

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");

        solver_writeInt(solver, 0, 6, in[t]);

        Des_SBox_6 sbox_6;
        sbox_6.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "DES_SBOX_6 UNSAT");
        mu_assert(out[t] == solver_readInt(solver, 6, 4), "DES_SBOX_6 failed");
    }
}
