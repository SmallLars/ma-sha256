#include "add_half_4.h"

#include "clausecreator.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Add_Half_4::stats[STATS_LENGTH];

Add_Half_4::Add_Half_4() : Modul(4, 2, 1) {
    output = 9;
}

Add_Half_4::~Add_Half_4() {
}

unsigned* Add_Half_4::getStats() {
    return stats;
}

void Add_Half_4::create(Printer* printer) {
    printer->newModul(3, "Add_Half_4", this);

    ClauseCreator cc(printer);
    //                c_out s_out[0]    s_out[1]    s_out[2]    s_out[3]    a_in[0]        a_in[1]        a_in[2]        a_in[3]    b_in[0]        b_in[1]        b_in[2]        b_in[3]
    cc.setLiterals(13, start, output, output + 1, output + 2, output + 3, inputs[0], inputs[0] + 1, inputs[0] + 2, inputs[0] + 3, inputs[1], inputs[1] + 1, inputs[1] + 2, inputs[1] + 3);
//    cc.printClause(13, CC_DC,      1,      0 ...........);

    cc.setLiterals(2, output, output);
    cc.printClause(2,      0,      1);

#ifdef ADDITIONAL_CLAUSES

#endif
}

MU_TEST_C(Add_Half_4::test) {
    for (unsigned a = 0; a < 16; a++) {
        for (unsigned b = 0; b < 16; b++) {
            SATSolver solver;
            solver.set_verbosity(0);
            solver.log_to_file("test.log");

            uint32_t ausgabe = a + b;

            solver_writeInt(solver, 0, 4, a);
            solver_writeInt(solver, 4, 4, b);

            Add_Half_4 adder;
            adder.append(&solver);

            lbool ret = solver.solve();
            mu_assert(ret == l_True, "HalfAdder UNSAT");
            mu_assert(((ausgabe >> 4) & 0x1) == solver_readInt(solver, 8, 1), "HalfAdder Carry failed");
            mu_assert(((ausgabe >> 0) & 0x15) == solver_readInt(solver, 9, 4), "HalfAdder Result failed");
        }
    }
}
