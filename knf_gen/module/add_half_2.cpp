#include "add_half_2.h"

#include "clausecreator.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Add_Half_2::stats[STATS_LENGTH];

Add_Half_2::Add_Half_2() : Modul(2, 2, 1) {
    output = 5;
}

Add_Half_2::~Add_Half_2() {
}

unsigned* Add_Half_2::getStats() {
    return stats;
}

void Add_Half_2::create(Printer* printer) {
    printer->newModul(1, "Add_Half_2", this);

    ClauseCreator cc(printer);
    //                c_out  s_out[0]  s_out[1]    a_in[0]        a_in[1]    b_in[0]        b_in[1]
    cc.setLiterals(7, start, output, output + 1, inputs[0], inputs[0] + 1, inputs[1], inputs[1] + 1);
    cc.printClause(7, CC_DC,      0,      CC_DC,         1,         CC_DC,         1,         CC_DC);
    cc.printClause(7,     1,  CC_DC,          1,     CC_DC,             0,     CC_DC,         CC_DC);
    cc.printClause(7,     0,  CC_DC,          0,     CC_DC,             1,     CC_DC,         CC_DC);
    cc.printClause(7, CC_DC,      1,      CC_DC,         1,         CC_DC,         0,         CC_DC);
    cc.printClause(7, CC_DC,      1,      CC_DC,         0,         CC_DC,         1,         CC_DC);
    cc.printClause(7,     0,      0,      CC_DC,     CC_DC,         CC_DC,     CC_DC,             1);
    cc.printClause(7,     0,  CC_DC,      CC_DC,     CC_DC,         CC_DC,         1,             1);
    cc.printClause(7, CC_DC,      0,          0,     CC_DC,             0,     CC_DC,             0);
    cc.printClause(7, CC_DC,      0,          1,     CC_DC,             1,     CC_DC,             0);
    cc.printClause(7,     1,  CC_DC,      CC_DC,         0,         CC_DC,         0,             0);
    cc.printClause(7, CC_DC,  CC_DC,          0,     CC_DC,             0,         1,             0);
    cc.printClause(7, CC_DC,  CC_DC,          1,     CC_DC,             1,         1,             0);
    cc.printClause(7, CC_DC,      0,          0,     CC_DC,             1,     CC_DC,             1);
    cc.printClause(7, CC_DC,  CC_DC,          0,     CC_DC,             1,         1,             1);
    cc.printClause(7, CC_DC,  CC_DC,          1,         0,             0,         0,             0);
    cc.printClause(7, CC_DC,  CC_DC,          0,         0,             0,         0,             1);
    cc.printClause(7, CC_DC,  CC_DC,          1,         0,             1,         0,             1);

#ifdef ADDITIONAL_CLAUSES
    //                  5       6        7           1               2          3             4
    //               c_out s_out[0]    s_out[1]    a_in[0]        a_in[1]    b_in[0]        b_in[1]
    cc.printClause(7,     0,  CC_DC,      CC_DC,         1,             1,     CC_DC,         CC_DC);
    cc.printClause(7,     0,  CC_DC,      CC_DC,         1,         CC_DC,     CC_DC,             1);
    cc.printClause(7,     0,  CC_DC,          0,         1,         CC_DC,     CC_DC,         CC_DC);
    cc.printClause(7,     0,  CC_DC,      CC_DC,     CC_DC,             1,         1,         CC_DC);
    cc.printClause(7,     0,      0,      CC_DC,     CC_DC,             1,     CC_DC,         CC_DC);
    cc.printClause(7,     0,  CC_DC,          0,     CC_DC,         CC_DC,         1,         CC_DC);
    cc.printClause(7,     0,      0,          0,     CC_DC,         CC_DC,     CC_DC,         CC_DC);
    cc.printClause(7,     1,  CC_DC,      CC_DC,         0,             0,         0,         CC_DC);
    cc.printClause(7, CC_DC,      0,          1,     CC_DC,             0,     CC_DC,             1);
    cc.printClause(7,     1,  CC_DC,          1,         0,         CC_DC,         0,         CC_DC);
    cc.printClause(7,     1,      1,          1,         0,         CC_DC,     CC_DC,         CC_DC);
    cc.printClause(7,     1,      1,          1,     CC_DC,         CC_DC,         0,         CC_DC);
#endif
}

MU_TEST_C(Add_Half_2::test) {
    for (unsigned a = 0; a < 4; a++) {
        for (unsigned b = 0; b < 4; b++) {
            SATSolver solver;
            solver.set_verbosity(0);
            solver.log_to_file("test.log");

            uint32_t ausgabe = a + b;

            solver_writeInt(solver, 0, 2, a);
            solver_writeInt(solver, 2, 2, b);

            Add_Half_2 adder;
            adder.append(&solver);

            lbool ret = solver.solve();
            mu_assert(ret == l_True, "HalfAdder UNSAT");
            mu_assert(((ausgabe >> 2) & 0x1) == solver_readInt(solver, 4, 1), "HalfAdder Carry failed");
            mu_assert(((ausgabe >> 0) & 0x3) == solver_readInt(solver, 5, 2), "HalfAdder Result failed");
        }
    }
}
