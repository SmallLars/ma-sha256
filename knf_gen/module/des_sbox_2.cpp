#include "des_sbox_2.h"

#include "clausecreator.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Des_SBox_2::stats[STATS_LENGTH];

Des_SBox_2::Des_SBox_2() : Modul(4, 1, 1) {
    setInputsBitWidth(6);
    output = 6;
}

Des_SBox_2::~Des_SBox_2() {
}

unsigned* Des_SBox_2::getStats() {
    return stats;
}

void Des_SBox_2::create(Printer* printer) {
    printer->newModul(0, "Des_SBox_2", this);

    ClauseCreator cc(printer);
    cc.setLiterals(10, inputs[0]+0, inputs[0]+1, inputs[0]+2, inputs[0]+3, inputs[0]+4, inputs[0]+5, output+0, output+1, output+2, output+3);
    cc.printClause(10,     1,     0, CC_DC, CC_DC, CC_DC,     1, CC_DC,     0,     0,     1);
    cc.printClause(10,     0, CC_DC, CC_DC,     1, CC_DC, CC_DC,     1,     1,     0,     1);
    cc.printClause(10, CC_DC,     1, CC_DC,     1, CC_DC,     0, CC_DC,     0, CC_DC,     1);
    cc.printClause(10, CC_DC,     0, CC_DC,     0,     0,     0, CC_DC,     0,     0, CC_DC);
    cc.printClause(10, CC_DC,     1, CC_DC, CC_DC,     1, CC_DC,     0,     0, CC_DC,     1);
    cc.printClause(10, CC_DC, CC_DC,     0,     1, CC_DC,     1, CC_DC,     0, CC_DC,     0);
    cc.printClause(10,     1, CC_DC, CC_DC,     0,     0,     1,     1, CC_DC, CC_DC,     0);
    cc.printClause(10,     0, CC_DC,     0, CC_DC,     1,     1,     1, CC_DC, CC_DC,     0);
    cc.printClause(10, CC_DC, CC_DC, CC_DC,     1,     1, CC_DC,     0,     1,     1,     1);
    cc.printClause(10,     1, CC_DC, CC_DC,     1,     1, CC_DC, CC_DC,     1, CC_DC,     1);
    cc.printClause(10, CC_DC,     0,     1,     0,     0,     1,     0, CC_DC, CC_DC,     1);
    cc.printClause(10,     1, CC_DC,     1, CC_DC,     0, CC_DC,     0, CC_DC,     0,     0);
    cc.printClause(10,     1, CC_DC,     1, CC_DC,     1,     1, CC_DC,     0,     1, CC_DC);
    cc.printClause(10,     0,     0,     1,     0, CC_DC,     1, CC_DC, CC_DC, CC_DC,     0);
    cc.printClause(10,     0, CC_DC,     0, CC_DC,     0, CC_DC, CC_DC,     0,     0,     1);
    cc.printClause(10, CC_DC,     1, CC_DC,     0,     0,     0,     0,     1, CC_DC, CC_DC);
    cc.printClause(10,     1,     0,     1, CC_DC, CC_DC,     0, CC_DC, CC_DC,     0,     0);
    cc.printClause(10, CC_DC, CC_DC, CC_DC, CC_DC,     1,     0,     0,     1,     1,     0);
    cc.printClause(10,     1, CC_DC, CC_DC, CC_DC,     0, CC_DC,     1,     1,     0,     0);
    cc.printClause(10,     1,     0,     0, CC_DC,     1, CC_DC,     1,     1, CC_DC, CC_DC);
    cc.printClause(10,     1,     0, CC_DC,     1, CC_DC, CC_DC,     1, CC_DC,     1,     0);
    cc.printClause(10, CC_DC,     1,     0, CC_DC,     0,     0, CC_DC,     1, CC_DC,     0);
    cc.printClause(10,     0,     0,     0,     1, CC_DC,     0, CC_DC,     1, CC_DC,     0);
    cc.printClause(10,     1, CC_DC,     0,     1, CC_DC,     0, CC_DC,     1,     0,     0);
    cc.printClause(10, CC_DC,     0,     0,     0, CC_DC, CC_DC,     0,     0,     1, CC_DC);
    cc.printClause(10, CC_DC,     0, CC_DC,     1,     0,     1,     0, CC_DC, CC_DC,     0);
    cc.printClause(10, CC_DC,     1,     0,     0,     0,     1, CC_DC, CC_DC,     0, CC_DC);
    cc.printClause(10,     0,     1,     1,     0,     1, CC_DC, CC_DC, CC_DC,     1, CC_DC);
    cc.printClause(10,     1, CC_DC,     0, CC_DC, CC_DC,     1,     0,     1,     1, CC_DC);
    cc.printClause(10,     0, CC_DC,     1, CC_DC, CC_DC,     1, CC_DC,     1,     1,     0);
    cc.printClause(10, CC_DC, CC_DC,     0,     1, CC_DC,     1,     1, CC_DC,     1,     0);
    cc.printClause(10,     1, CC_DC, CC_DC, CC_DC,     1, CC_DC,     0, CC_DC,     0,     1);
    cc.printClause(10,     1, CC_DC, CC_DC,     0,     0, CC_DC,     1, CC_DC,     0,     1);
    cc.printClause(10, CC_DC,     1,     1,     0, CC_DC, CC_DC,     1,     1,     0, CC_DC);
    cc.printClause(10,     0,     1,     1,     1, CC_DC, CC_DC, CC_DC,     1,     0, CC_DC);
    cc.printClause(10,     1,     0, CC_DC, CC_DC,     0, CC_DC, CC_DC,     0, CC_DC,     1);
    cc.printClause(10, CC_DC,     0,     1,     0,     1,     0, CC_DC, CC_DC, CC_DC,     1);
    cc.printClause(10,     1, CC_DC,     1, CC_DC,     1,     1,     1, CC_DC, CC_DC,     0);
    cc.printClause(10,     0, CC_DC, CC_DC,     1, CC_DC,     1,     1,     0,     0, CC_DC);
    cc.printClause(10,     1, CC_DC,     0,     0,     0,     0, CC_DC,     1, CC_DC, CC_DC);
    cc.printClause(10,     1,     1,     1, CC_DC,     1,     0,     0, CC_DC, CC_DC, CC_DC);
    cc.printClause(10,     0,     1,     0, CC_DC,     1,     0,     0, CC_DC, CC_DC, CC_DC);
    cc.printClause(10,     0,     1, CC_DC, CC_DC,     0,     0, CC_DC,     0,     1, CC_DC);
    cc.printClause(10,     0,     1,     1, CC_DC, CC_DC,     0, CC_DC,     0,     0, CC_DC);
    cc.printClause(10, CC_DC,     0, CC_DC, CC_DC, CC_DC,     0,     0,     1,     1,     1);
    cc.printClause(10,     1,     1,     1, CC_DC,     0,     0, CC_DC, CC_DC, CC_DC,     1);
    cc.printClause(10,     0,     0, CC_DC,     0, CC_DC,     0, CC_DC,     1,     0, CC_DC);
    cc.printClause(10,     1, CC_DC,     1,     1, CC_DC,     0,     1,     0, CC_DC, CC_DC);
    cc.printClause(10, CC_DC,     0, CC_DC, CC_DC,     1,     1, CC_DC,     1,     1,     1);
    cc.printClause(10, CC_DC, CC_DC, CC_DC, CC_DC,     1,     1,     1,     0,     1,     1);
    cc.printClause(10,     1, CC_DC,     0, CC_DC, CC_DC,     0,     1, CC_DC,     1,     1);
    cc.printClause(10, CC_DC, CC_DC, CC_DC,     0,     1, CC_DC,     1,     0,     0,     1);
    cc.printClause(10, CC_DC, CC_DC, CC_DC,     1,     1,     1, CC_DC,     1,     0,     0);
    cc.printClause(10,     0, CC_DC, CC_DC,     1,     1,     0,     1,     0, CC_DC, CC_DC);
    cc.printClause(10,     1, CC_DC, CC_DC, CC_DC,     1,     0, CC_DC,     0,     1,     0);
    cc.printClause(10, CC_DC,     1, CC_DC,     1, CC_DC,     0,     1, CC_DC,     1, CC_DC);
    cc.printClause(10, CC_DC,     1,     0, CC_DC,     1, CC_DC,     1,     1, CC_DC,     1);
    cc.printClause(10, CC_DC, CC_DC,     1, CC_DC,     0, CC_DC,     1,     0,     1, CC_DC);
    cc.printClause(10, CC_DC, CC_DC, CC_DC,     0,     1,     1,     0, CC_DC,     0, CC_DC);
    cc.printClause(10,     0,     0,     0, CC_DC,     0, CC_DC, CC_DC, CC_DC, CC_DC,     1);
    cc.printClause(10, CC_DC,     0,     1, CC_DC,     0,     0, CC_DC, CC_DC, CC_DC,     0);
    cc.printClause(10,     0,     0, CC_DC,     0, CC_DC, CC_DC,     1, CC_DC,     1, CC_DC);
    cc.printClause(10,     0,     0, CC_DC,     1, CC_DC, CC_DC,     0,     0, CC_DC, CC_DC);
    cc.printClause(10,     1, CC_DC,     0,     1,     0, CC_DC, CC_DC,     0, CC_DC, CC_DC);
    cc.printClause(10,     1,     0, CC_DC, CC_DC, CC_DC, CC_DC,     0,     0,     0, CC_DC);
    cc.printClause(10,     0,     0,     1, CC_DC, CC_DC, CC_DC,     1,     1, CC_DC, CC_DC);
    cc.printClause(10, CC_DC, CC_DC,     0,     1, CC_DC,     0,     0, CC_DC, CC_DC,     1);
    cc.printClause(10,     1, CC_DC,     1,     1, CC_DC,     1, CC_DC, CC_DC, CC_DC,     1);
    cc.printClause(10, CC_DC, CC_DC,     0, CC_DC, CC_DC, CC_DC,     1,     0,     0,     0);
    cc.printClause(10, CC_DC,     1, CC_DC, CC_DC, CC_DC, CC_DC,     0,     0,     1,     1);
    cc.printClause(10, CC_DC,     1, CC_DC, CC_DC, CC_DC, CC_DC,     0,     1,     1,     0);
    cc.printClause(10, CC_DC, CC_DC, CC_DC,     0, CC_DC,     0,     0,     0, CC_DC,     0);
    cc.printClause(10, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC,     1,     1,     1,     1);
    cc.printClause(10,     1,     1, CC_DC,     0, CC_DC, CC_DC, CC_DC, CC_DC,     1,     0);
    cc.printClause(10, CC_DC,     0, CC_DC,     0, CC_DC,     1, CC_DC,     1, CC_DC,     0);
    cc.printClause(10, CC_DC,     1, CC_DC, CC_DC,     0,     1, CC_DC,     0, CC_DC,     0);
    cc.printClause(10, CC_DC,     0, CC_DC,     1, CC_DC, CC_DC, CC_DC,     1,     0,     1);
    cc.printClause(10,     0,     1, CC_DC,     0, CC_DC,     1, CC_DC, CC_DC, CC_DC,     1);
    cc.printClause(10,     0,     1, CC_DC,     1, CC_DC,     1, CC_DC, CC_DC, CC_DC,     0);
    cc.printClause(10, CC_DC,     1, CC_DC, CC_DC,     0,     1, CC_DC,     1, CC_DC,     1);
    cc.printClause(10, CC_DC,     1, CC_DC,     0,     1,     0, CC_DC, CC_DC, CC_DC,     0);
}

MU_TEST_C(Des_SBox_2::test) {
    unsigned in[]  = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                      0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                      0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                      0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
                      0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
                      0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
                      0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
                      0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f};
    unsigned out[]  = {0x0f, 0x03, 0x01, 0x0d, 0x08, 0x04, 0x0e, 0x07,
                       0x06, 0x0f, 0x0b, 0x02, 0x03, 0x08, 0x04, 0x0e,
                       0x09, 0x0c, 0x07, 0x00, 0x02, 0x01, 0x0d, 0x0a,
                       0x0c, 0x06, 0x00, 0x09, 0x05, 0x0b, 0x0a, 0x05,
                       0x00, 0x0d, 0x0e, 0x08, 0x07, 0x0a, 0x0b, 0x01,
                       0x0a, 0x03, 0x04, 0x0f, 0x0d, 0x04, 0x01, 0x02,
                       0x05, 0x0b, 0x08, 0x06, 0x0c, 0x07, 0x06, 0x0c,
                       0x09, 0x00, 0x03, 0x05, 0x02, 0x0e, 0x0f, 0x09};

    for (unsigned t = 0; t < 64; t++) {
        SATSolver solver;
        solver.set_verbosity(0);
        solver.log_to_file("test.log");

        solver_writeInt_msb(solver, 0, 6, in[t]);

        Des_SBox_2 sbox_2;
        sbox_2.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "DES_SBOX_2 UNSAT");
        mu_assert(out[t] == solver_readInt_msb(solver, 6, 4), "DES_SBOX_2 failed");
    }
}
