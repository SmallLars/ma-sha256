#include "add_full_1.h"

#include "clausecreator.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Add_Full_1::stats[STATS_LENGTH];

Add_Full_1::Add_Full_1() : Modul(1, 3, 1) {
    inputs.push_back(0);
    inputs.push_back(1);
    inputs.push_back(2);
    start = 3;
    output = 4;
}

Add_Full_1::~Add_Full_1() {
}

unsigned* Add_Full_1::getStats() {
    return stats;
}

void Add_Full_1::create(Printer* printer) {
    ClauseCreator cc(printer);
#ifdef XOR_OPTIMIZATION
    //                c_out       a_in       b_in       c_in
    cc.setLiterals(4, start, inputs[0], inputs[1], inputs[2]);
    cc.printClause(4,     1,         0,         0,     CC_DC);
    cc.printClause(4,     0,         1,         1,     CC_DC);
    cc.printClause(4,     1,         0,     CC_DC,         0);
    cc.printClause(4,     1,     CC_DC,         0,         0);
    cc.printClause(4,     0,         1,     CC_DC,         1);
    cc.printClause(4,     0,     CC_DC,         1,          1);

    // XOR ->          !s_out       a_in       b_in       c_in
    createXOR(printer, output, inputs[0], inputs[1], inputs[2]);
#else
    //                c_out   s_out       a_in       b_in       c_in
    cc.setLiterals(5, start, output, inputs[0], inputs[1], inputs[2]);
    cc.printClause(5, CC_DC,      1,         0,         0,         0);
    cc.printClause(5, CC_DC,      0,         1,         1,         1);
    cc.printClause(5,     1,  CC_DC,         0,         0,     CC_DC);
    cc.printClause(5,     0,  CC_DC,         1,         1,     CC_DC);
    cc.printClause(5,     1,      1,     CC_DC,     CC_DC,         0);
    cc.printClause(5,     0,      0,     CC_DC,     CC_DC,         1);
    cc.printClause(5, CC_DC,      0,         1,         0,         0);
    cc.printClause(5, CC_DC,      0,         0,         1,         0);
    cc.printClause(5, CC_DC,      1,         1,         0,         1);
    cc.printClause(5, CC_DC,      1,         0,         1,         1);
#endif
}

MU_TEST_C(Add_Full_1::test) {
    unsigned a[] = {0, 0, 1, 1, 0, 0, 1, 1};
    unsigned b[] = {0, 1, 0, 1, 0, 1, 0, 1};
    unsigned c[] = {0, 0, 0, 0, 1, 1, 1, 1};

    for (unsigned t = 0; t < 8; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");

        uint32_t ausgabe = a[t] + b[t] + c[t];

        solver_writeInt(solver, 0, 1, a[t]);
        solver_writeInt(solver, 1, 1, b[t]);
        solver_writeInt(solver, 2, 1, c[t]);

        Add_Full_1 adder;
        adder.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "FullAdder UNSAT");
        mu_assert(((ausgabe >> 1) & 1) == solver_readInt(solver, 3, 1), "FullAdder Carry failed");
        mu_assert(((ausgabe >> 0) & 1) == solver_readInt(solver, 4, 1), "FullAdder Result failed");
    }
}
