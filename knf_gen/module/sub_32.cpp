#include "sub_32.h"

#include "clausecreator.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Sub_32::stats[STATS_LENGTH];

Sub_32::Sub_32() : Modul(32, 2, 1) {
    output = 95;
}

Sub_32::~Sub_32() {
}

unsigned* Sub_32::getStats() {
    return stats;
}

void Sub_32::create(Printer* printer) {
    printer->newModul(10, "Sub_32", this);

    ClauseCreator cc(printer);

    // Half subber
#ifdef XOR_SUPPORT
    // XOR ->          !d_out       a_in       b_in
    createXOR(printer, output, inputs[0], inputs[1]);

    // AND ->          c_out   d_out       b_in
    createAND(printer, start, output, inputs[1]);
#else
    //                 d_out  c_out       a_in       b_in
    cc.setLiterals(4, output, start, inputs[0], inputs[1]);
    cc.printClause(4,      0, CC_DC,         0,         0);
    cc.printClause(4,      1, CC_DC,         0,         1);
    cc.printClause(4,  CC_DC,     1,         1,         0);
    cc.printClause(4,      0, CC_DC,         1,         1);
    cc.printClause(4,  CC_DC,     0,         0,     CC_DC);
    cc.printClause(4,      1,     0,     CC_DC,     CC_DC);
#endif

    // Full subber x30
    for (unsigned i = 1; i < 31; i++) {
#ifdef XOR_SUPPORT
        //                    c_out           a_in           b_in           c_in
        cc.setLiterals(4, start + i, inputs[0] + i, inputs[1] + i, start - 1 + i);
        cc.printClause(4,         0,             0,             1,         CC_DC);
        cc.printClause(4,         0,             0,         CC_DC,             1);
        cc.printClause(4,         1,             1,             0,         CC_DC);
        cc.printClause(4,         1,             1,         CC_DC,             0);
        cc.printClause(4,         1,         CC_DC,             0,             0);
        cc.printClause(4,         0,         CC_DC,             1,             1);

        // XOR ->              !d_out           a_in           b_in           c_in
        createXOR(printer, output + i, inputs[0] + i, inputs[1] + i, start - 1 + i);
#else
        //                     c_out      d_out           a_in           b_in           c_in
        cc.setLiterals(5, output + i, start + i, inputs[0] + i, inputs[1] + i, start - 1 + i);
        cc.printClause(5,          0,     CC_DC,             0,             0,             1);
        cc.printClause(5,          0,     CC_DC,             0,             1,             0);
        cc.printClause(5,          1,     CC_DC,             0,             1,             1);
        cc.printClause(5,          1,         0,             0,         CC_DC,         CC_DC);
        cc.printClause(5,          0,     CC_DC,             1,             0,             0);
        cc.printClause(5,          1,     CC_DC,             1,             0,             1);
        cc.printClause(5,          1,     CC_DC,             1,             1,             0);
        cc.printClause(5,          0,         1,             1,         CC_DC,         CC_DC);
        cc.printClause(5,      CC_DC,         1,         CC_DC,             0,             0);
        cc.printClause(5,      CC_DC,         0,         CC_DC,             1,             1);
#endif
    }

    // Final subber (without carry calculation)
    // XOR ->               !d_out            a_in            b_in        c_in
    createXOR(printer, output + 31, inputs[0] + 31, inputs[1] + 31, start + 30);
}

MU_TEST_C(Sub_32::test) {
    unsigned a[] = {1234, 5, 0x80000000, 1, 0xFFFFFFFF, 0x2, 0xFFFFFFFF, 0x1, 0xFFFFFFFF, 0x0};
    unsigned b[] = {1235, 6, 1, 0x80000000, 0x2, 0xFFFFFFFF, 0x1, 0xFFFFFFFF, 0x0, 0xFFFFFFFF};

    for (unsigned t = 0; t < 10; t++) {
        SATSolver solver;
        solver.set_verbosity(0);
        solver.log_to_file("test.log");

        uint32_t ausgabe = a[t] - b[t];

        solver_writeInt(solver,  0, 32, a[t]);
        solver_writeInt(solver, 32, 32, b[t]);

        Sub_32 adder;
        adder.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "Subber UNSAT");
        mu_assert(ausgabe == solver_readInt(solver, 95, 32), "Subber failed");
    }
}
