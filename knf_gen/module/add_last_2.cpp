#include "add_last_2.h"

#include "clausecreator.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Add_Last_2::stats[STATS_LENGTH];
static bool fullCNF = false;

Add_Last_2::Add_Last_2() : Modul(2, 3, 1) {
    setInputsBitWidth(2, 2, 1);
    output = 5;
}

Add_Last_2::~Add_Last_2() {
}

unsigned* Add_Last_2::getStats() {
    return stats;
}

void Add_Last_2::create(Collector* collector) {
    collector->newModul(1, "Add_Last_2", this);

    ClauseCreator cc(collector);
    //              s_out[0]    s_out[1]    a_in[0]        a_in[1]    b_in[0]        b_in[1]       c_in
    cc.setLiterals(7, output, output + 1, inputs[0], inputs[0] + 1, inputs[1], inputs[1] + 1, inputs[2]);

    //                     6           7          1              2          3              4          5
    cc.printClause(7,  CC_DC,          0,         1,             1,         1,             1,     CC_DC);
    cc.printClause(7,  CC_DC,          1,         1,             1,         1,             0,     CC_DC);
    cc.printClause(7,  CC_DC,          1,         1,             0,         1,             1,     CC_DC);
    cc.printClause(7,  CC_DC,          0,         1,             0,         1,             0,     CC_DC);
    cc.printClause(7,  CC_DC,          1,         0,             1,         0,             1,     CC_DC);
    cc.printClause(7,  CC_DC,          0,         0,             1,         0,             0,     CC_DC);
    cc.printClause(7,  CC_DC,          0,         0,             0,         0,             1,     CC_DC);
    cc.printClause(7,  CC_DC,          1,         0,             0,         0,             0,     CC_DC);
    cc.printClause(7,      0,          0,     CC_DC,             1,         1,             1,     CC_DC);
    cc.printClause(7,      0,          1,     CC_DC,             1,         1,             0,     CC_DC);
    cc.printClause(7,      1,          1,     CC_DC,             1,         0,             1,     CC_DC);
    cc.printClause(7,      1,          0,     CC_DC,             1,         0,             0,     CC_DC);
    cc.printClause(7,      0,          1,     CC_DC,             0,         1,             1,     CC_DC);
    cc.printClause(7,      0,          0,     CC_DC,             0,         1,             0,     CC_DC);
    cc.printClause(7,      1,          0,     CC_DC,             0,         0,             1,     CC_DC);
    cc.printClause(7,      1,          1,     CC_DC,             0,         0,             0,     CC_DC);
    cc.printClause(7,      0,          0,     CC_DC,             1,     CC_DC,             1,         1);
    cc.printClause(7,      1,          1,     CC_DC,             1,     CC_DC,             1,         0);
    cc.printClause(7,      0,          1,     CC_DC,             1,     CC_DC,             0,         1);
    cc.printClause(7,      1,          0,     CC_DC,             1,     CC_DC,             0,         0);
    cc.printClause(7,      0,          1,     CC_DC,             0,     CC_DC,             1,         1);
    cc.printClause(7,      1,          0,     CC_DC,             0,     CC_DC,             1,         0);
    cc.printClause(7,      0,          0,     CC_DC,             0,     CC_DC,             0,         1);
    cc.printClause(7,      1,          1,     CC_DC,             0,     CC_DC,             0,         0);

    if (!fullCNF) return;

    cc.printClause(7,      1,      CC_DC,         0,         CC_DC,         0,         CC_DC,         0);
    cc.printClause(7,      0,      CC_DC,         1,         CC_DC,         1,         CC_DC,         1);
    cc.printClause(7,      0,      CC_DC,         1,         CC_DC,         0,         CC_DC,         0);
    cc.printClause(7,      0,      CC_DC,         0,         CC_DC,         1,         CC_DC,         0);
    cc.printClause(7,      1,      CC_DC,         1,         CC_DC,         0,         CC_DC,         1);
    cc.printClause(7,      1,      CC_DC,         0,         CC_DC,         1,         CC_DC,         1);
    cc.printClause(7,  CC_DC,          1,         0,             0,         0,             0,     CC_DC);
    cc.printClause(7,  CC_DC,          0,         0,             1,         0,             0,     CC_DC);
    cc.printClause(7,  CC_DC,          0,         1,             0,         1,             0,     CC_DC);
    cc.printClause(7,  CC_DC,          1,         1,             1,         1,             0,     CC_DC);
    cc.printClause(7,  CC_DC,          0,         0,             0,         0,             1,     CC_DC);
    cc.printClause(7,  CC_DC,          1,         0,             1,         0,             1,     CC_DC);
    cc.printClause(7,  CC_DC,          1,         1,             0,         1,             1,     CC_DC);
    cc.printClause(7,  CC_DC,          0,         1,             1,         1,             1,     CC_DC);
    cc.printClause(7,      1,          1,     CC_DC,             0,     CC_DC,             0,         0);
    cc.printClause(7,      1,          0,     CC_DC,             1,     CC_DC,             0,         0);
    cc.printClause(7,      1,          0,     CC_DC,             0,     CC_DC,             1,         0);
    cc.printClause(7,      1,          1,     CC_DC,             1,     CC_DC,             1,         0);
    cc.printClause(7,      0,          0,     CC_DC,             0,     CC_DC,             0,         1);
    cc.printClause(7,      0,          1,     CC_DC,             1,     CC_DC,             0,         1);
    cc.printClause(7,      0,          1,     CC_DC,             0,     CC_DC,             1,         1);
    cc.printClause(7,      0,          0,     CC_DC,             1,     CC_DC,             1,         1);
}

MU_TEST_C(Add_Last_2::test) {
    for (unsigned a = 0; a < 4; a++) {
        for (unsigned b = 0; b < 4; b++) {
            for (unsigned c = 0; c < 2; c++) {
                SATSolver solver;
                solver.set_verbosity(0);
                solver.log_to_file("test.log");

                uint32_t ausgabe = a + b + c;

                solver_writeInt(solver, 0, 2, a);
                solver_writeInt(solver, 2, 2, b);
                solver_writeInt(solver, 4, 1, c);

                Add_Last_2 adder;
                fullCNF = true;
                adder.append(&solver);
                fullCNF = false;

                lbool ret = solver.solve();
                mu_assert(ret == l_True, "LastAdder UNSAT");
                mu_assert(((ausgabe >> 0) & 0x3) == solver_readInt(solver, 5, 2), "LastAdder Result failed");
            }
        }
    }
}
