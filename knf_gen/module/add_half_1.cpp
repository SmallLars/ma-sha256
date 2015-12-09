#include "add_half_1.h"

#include "clausecreator.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Add_Half_1::stats[STATS_LENGTH];

Add_Half_1::Add_Half_1() : Modul(1, 2, 1) {
    output = 3;
}

Add_Half_1::~Add_Half_1() {
}

unsigned* Add_Half_1::getStats() {
    return stats;
}

void Add_Half_1::create(Printer* printer) {
    printer->newModul(0, "Add_Half_1", this);

    ClauseCreator cc(printer);
#ifdef XOR_OPTIMIZATION
    // AND ->          c_out       a_in       b_in
    createAND(printer, start, inputs[0], inputs[1]);

    // XOR ->          !s_out       a_in       b_in
    createXOR(printer, output, inputs[0], inputs[1]);
#else
    //                 c_out  s_out       a_in       b_in
    cc.setLiterals(4, start, output, inputs[0], inputs[1]);
    cc.printClause(4, CC_DC,      0,         1,         1);
    cc.printClause(4,     0,      0,     CC_DC,     CC_DC);
    cc.printClause(4,     0,  CC_DC,     CC_DC,         1);
    cc.printClause(4,     1,  CC_DC,         0,         0);
    cc.printClause(4, CC_DC,      1,         1,         0);
    cc.printClause(4, CC_DC,      1,         0,         1);
#endif

#ifdef ADDITIONAL_CLAUSES
    //                   3      4           1          2
    //                 c_out  s_out       a_in       b_in
    cc.setLiterals(4, start, output, inputs[0], inputs[1]);
    cc.printClause(4,     1,      1,         0,     CC_DC);
    cc.printClause(4,     0,  CC_DC,         1,         0);
    cc.printClause(4,     0,  CC_DC,         0,         1);
    cc.printClause(4,     1,      1,     CC_DC,         0);
#endif
}

MU_TEST_C(Add_Half_1::test) {
    for (unsigned a = 0; a < 2; a++) {
        for (unsigned b = 0; b < 2; b++) {
            SATSolver solver;
            solver.set_verbosity(0);
            solver.log_to_file("test.log");

            uint32_t ausgabe = a + b;

            solver_writeInt(solver, 0, 1, a);
            solver_writeInt(solver, 1, 1, b);

            Add_Half_1 adder;
            adder.append(&solver);

            lbool ret = solver.solve();
            mu_assert(ret == l_True, "HalfAdder UNSAT");
            mu_assert(((ausgabe >> 1) & 0x1) == solver_readInt(solver, 2, 1), "HalfAdder Carry failed");
            mu_assert(((ausgabe >> 0) & 0x1) == solver_readInt(solver, 3, 1), "HalfAdder Result failed");
        }
    }
}
