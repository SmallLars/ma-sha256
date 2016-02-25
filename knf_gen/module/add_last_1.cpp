#include "add_last_1.h"

#include "clausecreator.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Add_Last_1::stats[STATS_LENGTH];

Add_Last_1::Add_Last_1() : Modul(1, 3, 1) {
    output = 3;
}

Add_Last_1::~Add_Last_1() {
}

unsigned* Add_Last_1::getStats() {
    return stats;
}

void Add_Last_1::create(Collector* collector) {
    collector->newModul(0, "Add_Last_1", this);

    // XOR ->          !s_out       a_in       b_in       c_in
    createXOR(collector, output, inputs[0], inputs[1], inputs[2]);

#ifdef ADDITIONAL_CLAUSES

#endif
}

MU_TEST_C(Add_Last_1::test) {
    for (unsigned a = 0; a < 2; a++) {
        for (unsigned b = 0; b < 2; b++) {
            for (unsigned c = 0; c < 2; c++) {
                SATSolver solver;
                solver.set_verbosity(0);
                solver.log_to_file("test.log");

                uint32_t ausgabe = a + b + c;

                solver_writeInt(solver, 0, 1, a);
                solver_writeInt(solver, 1, 1, b);
                solver_writeInt(solver, 2, 1, c);

                Add_Last_1 adder;
                adder.append(&solver);

                lbool ret = solver.solve();
                mu_assert(ret == l_True, "LastAdder UNSAT");
                mu_assert(((ausgabe >> 0) & 0x1) == solver_readInt(solver, 3, 1), "LastAdder Result failed");
            }
        }
    }
}
