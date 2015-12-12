#include "add_full_3.h"

#include "clausecreator.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Add_Full_3::stats[STATS_LENGTH];
static bool fullCNF = false;

Add_Full_3::Add_Full_3() : Modul(3, 3, 1) {
    setInputsBitWidth(3, 3, 1);
    output = 8;
}

Add_Full_3::~Add_Full_3() {
}

unsigned* Add_Full_3::getStats() {
    return stats;
}

void Add_Full_3::create(Printer* printer) {
    printer->newModul(2, "Add_Full_3", this);

    ClauseCreator cc(printer);
    //                 c_out  s_out[0]  s_out[1]    s_out[2]    a_in[0]        a_in[1]        a_in[2]    b_in[0]        b_in[1]        b_in[2]       c_in
    cc.setLiterals(11, start, output, output + 1, output + 2, inputs[0], inputs[0] + 1, inputs[0] + 2, inputs[1], inputs[1] + 1, inputs[1] + 2, inputs[2]);

    //                     8       9          10          11          1              2              3          4              5              6          7
    cc.printClause(11,     0,      0,      CC_DC,      CC_DC,         1,             1,             1,     CC_DC,         CC_DC,         CC_DC,     CC_DC);
    cc.printClause(11,     0,  CC_DC,      CC_DC,      CC_DC,         1,             1,         CC_DC,         1,         CC_DC,             1,     CC_DC);
    cc.printClause(11,     1,  CC_DC,      CC_DC,      CC_DC,         0,             0,         CC_DC,         0,         CC_DC,             0,     CC_DC);
    cc.printClause(11,     0,      0,      CC_DC,      CC_DC,         1,             1,         CC_DC,     CC_DC,         CC_DC,             1,     CC_DC);
    cc.printClause(11,     0,      0,      CC_DC,          0,         1,             1,         CC_DC,     CC_DC,         CC_DC,         CC_DC,     CC_DC);
    cc.printClause(11,     1,  CC_DC,          1,          1,         0,         CC_DC,         CC_DC,     CC_DC,         CC_DC,         CC_DC,         0);
    cc.printClause(11,     1,      1,          1,          1,         0,         CC_DC,         CC_DC,     CC_DC,         CC_DC,         CC_DC,     CC_DC);
    cc.printClause(11,     0,      0,      CC_DC,      CC_DC,     CC_DC,             1,         CC_DC,     CC_DC,         CC_DC,             1,         1);
    cc.printClause(11,     1,      1,      CC_DC,      CC_DC,     CC_DC,             0,         CC_DC,     CC_DC,         CC_DC,             0,         0);
    cc.printClause(11,     1,      1,          1,      CC_DC,     CC_DC,         CC_DC,             0,     CC_DC,         CC_DC,         CC_DC,         0);
    cc.printClause(11,     0,      0,          0,      CC_DC,     CC_DC,         CC_DC,         CC_DC,         1,         CC_DC,             1,     CC_DC);
    cc.printClause(11,     1,  CC_DC,          1,          1,     CC_DC,         CC_DC,         CC_DC,         0,         CC_DC,         CC_DC,         0);
    cc.printClause(11,     1,      1,          1,      CC_DC,     CC_DC,         CC_DC,         CC_DC,     CC_DC,         CC_DC,             0,         0);
    cc.printClause(11,     1,      1,          1,          1,     CC_DC,         CC_DC,         CC_DC,     CC_DC,         CC_DC,         CC_DC,         0);
    cc.printClause(11, CC_DC,  CC_DC,      CC_DC,          0,         1,             1,             1,         1,         CC_DC,             1,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,      CC_DC,          1,         1,             1,             1,         1,         CC_DC,             0,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,      CC_DC,          1,         1,             1,             0,         1,         CC_DC,             1,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,      CC_DC,          0,         1,             1,             0,         1,         CC_DC,             0,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,      CC_DC,          1,         0,             0,             1,         0,         CC_DC,             1,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,      CC_DC,          0,         0,             0,             1,         0,         CC_DC,             0,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,      CC_DC,          0,         0,             0,             0,         0,         CC_DC,             1,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,      CC_DC,          1,         0,             0,             0,         0,         CC_DC,             0,     CC_DC);
    cc.printClause(11, CC_DC,      0,      CC_DC,          0,         1,             1,             1,     CC_DC,         CC_DC,             1,     CC_DC);
    cc.printClause(11, CC_DC,      0,      CC_DC,          1,         1,             1,             0,     CC_DC,         CC_DC,             1,     CC_DC);
    cc.printClause(11, CC_DC,      0,      CC_DC,          0,         1,             1,             0,     CC_DC,         CC_DC,             0,     CC_DC);
    cc.printClause(11, CC_DC,      1,          1,          1,         0,         CC_DC,             0,     CC_DC,         CC_DC,             0,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,      CC_DC,          0,     CC_DC,             1,             1,         1,         CC_DC,             1,         1);
    cc.printClause(11, CC_DC,  CC_DC,      CC_DC,          1,     CC_DC,             1,             0,         1,         CC_DC,             1,         1);
    cc.printClause(11, CC_DC,  CC_DC,      CC_DC,          0,     CC_DC,             1,             0,         1,         CC_DC,             0,         1);
    cc.printClause(11, CC_DC,      1,      CC_DC,          1,     CC_DC,             0,             1,         0,         CC_DC,             1,     CC_DC);
    cc.printClause(11, CC_DC,      1,      CC_DC,          0,     CC_DC,             0,             1,         0,         CC_DC,             0,     CC_DC);
    cc.printClause(11, CC_DC,      1,      CC_DC,          0,     CC_DC,             0,             0,         0,         CC_DC,             1,     CC_DC);
    cc.printClause(11, CC_DC,      1,      CC_DC,          1,     CC_DC,             0,             0,         0,         CC_DC,             0,     CC_DC);
    cc.printClause(11, CC_DC,      0,      CC_DC,          0,     CC_DC,             1,             1,     CC_DC,         CC_DC,             1,         1);
    cc.printClause(11, CC_DC,      0,      CC_DC,          1,     CC_DC,             1,             1,     CC_DC,         CC_DC,             0,         1);
    cc.printClause(11, CC_DC,      0,      CC_DC,          1,     CC_DC,             1,             0,     CC_DC,         CC_DC,             1,         1);
    cc.printClause(11, CC_DC,      0,      CC_DC,          0,     CC_DC,             1,             0,     CC_DC,         CC_DC,             0,         1);
    cc.printClause(11, CC_DC,      1,      CC_DC,          1,     CC_DC,             0,             1,     CC_DC,         CC_DC,             1,         0);
    cc.printClause(11, CC_DC,      1,      CC_DC,          0,     CC_DC,             0,             1,     CC_DC,         CC_DC,             0,         0);
    cc.printClause(11, CC_DC,      1,      CC_DC,          0,     CC_DC,             0,             0,     CC_DC,         CC_DC,             1,         0);
    cc.printClause(11, CC_DC,      1,      CC_DC,          1,     CC_DC,             0,             0,     CC_DC,         CC_DC,             0,         0);
    cc.printClause(11, CC_DC,      0,          0,          0,     CC_DC,         CC_DC,             1,         1,         CC_DC,             1,     CC_DC);
    cc.printClause(11, CC_DC,      1,          1,          0,     CC_DC,         CC_DC,             1,         0,         CC_DC,             0,     CC_DC);
    cc.printClause(11, CC_DC,      1,          1,          1,     CC_DC,         CC_DC,             1,     CC_DC,         CC_DC,             1,         0);
    cc.printClause(11, CC_DC,      1,          1,          0,     CC_DC,         CC_DC,             1,     CC_DC,         CC_DC,             0,         0);
    cc.printClause(11, CC_DC,      1,          1,          0,     CC_DC,         CC_DC,             0,     CC_DC,         CC_DC,             1,         0);
    cc.printClause(11, CC_DC,      1,          1,          1,     CC_DC,         CC_DC,             0,     CC_DC,         CC_DC,             0,         0);

    if (!fullCNF) return;

    cc.printClause(11, CC_DC,      1,      CC_DC,      CC_DC,         0,         CC_DC,         CC_DC,         0,         CC_DC,         CC_DC,         0);
    cc.printClause(11, CC_DC,      0,      CC_DC,      CC_DC,         1,         CC_DC,         CC_DC,         1,         CC_DC,         CC_DC,         1);
    cc.printClause(11,     1,  CC_DC,      CC_DC,          1,     CC_DC,         CC_DC,             0,     CC_DC,         CC_DC,         CC_DC,     CC_DC);
    cc.printClause(11,     0,  CC_DC,      CC_DC,          0,     CC_DC,         CC_DC,             1,     CC_DC,         CC_DC,         CC_DC,     CC_DC);
    cc.printClause(11,     1,  CC_DC,          1,      CC_DC,     CC_DC,         CC_DC,         CC_DC,     CC_DC,             0,             0,     CC_DC);
    cc.printClause(11,     0,  CC_DC,          0,      CC_DC,     CC_DC,         CC_DC,         CC_DC,     CC_DC,             1,             1,     CC_DC);
    cc.printClause(11, CC_DC,      0,      CC_DC,      CC_DC,         1,         CC_DC,         CC_DC,         0,         CC_DC,         CC_DC,         0);
    cc.printClause(11, CC_DC,      0,      CC_DC,      CC_DC,         0,         CC_DC,         CC_DC,         1,         CC_DC,         CC_DC,         0);
    cc.printClause(11, CC_DC,      1,      CC_DC,      CC_DC,         1,         CC_DC,         CC_DC,         0,         CC_DC,         CC_DC,         1);
    cc.printClause(11, CC_DC,      1,      CC_DC,      CC_DC,         0,         CC_DC,         CC_DC,         1,         CC_DC,         CC_DC,         1);
    cc.printClause(11, CC_DC,  CC_DC,          1,      CC_DC,         0,             0,         CC_DC,         0,             0,         CC_DC,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,          0,      CC_DC,         0,             1,         CC_DC,         0,             0,         CC_DC,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,          0,      CC_DC,         1,             0,         CC_DC,         1,             0,         CC_DC,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,          1,      CC_DC,         0,             1,         CC_DC,         0,             1,         CC_DC,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,          1,      CC_DC,         1,             0,         CC_DC,         1,             1,         CC_DC,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,          0,      CC_DC,         1,             1,         CC_DC,         1,             1,         CC_DC,     CC_DC);
    cc.printClause(11,     1,  CC_DC,      CC_DC,      CC_DC,         0,             0,         CC_DC,         0,         CC_DC,             0,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,          1,          1,     CC_DC,         CC_DC,             0,     CC_DC,             0,             0,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,          0,          0,     CC_DC,         CC_DC,             0,     CC_DC,             1,             0,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,          0,          1,     CC_DC,         CC_DC,             1,     CC_DC,             1,             0,     CC_DC);
    cc.printClause(11,     0,  CC_DC,      CC_DC,      CC_DC,         1,             1,         CC_DC,         1,         CC_DC,             1,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,          1,          0,     CC_DC,         CC_DC,             0,     CC_DC,             0,             1,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,          1,          1,     CC_DC,         CC_DC,             1,     CC_DC,             0,             1,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,          0,          0,     CC_DC,         CC_DC,             1,     CC_DC,             1,             1,     CC_DC);
    cc.printClause(11, CC_DC,      1,          1,      CC_DC,     CC_DC,             0,         CC_DC,     CC_DC,             0,         CC_DC,         0);
    cc.printClause(11, CC_DC,      1,          0,      CC_DC,     CC_DC,             1,         CC_DC,     CC_DC,             0,         CC_DC,         0);
    cc.printClause(11, CC_DC,      1,          1,      CC_DC,     CC_DC,             1,         CC_DC,     CC_DC,             1,         CC_DC,         0);
    cc.printClause(11,     1,      1,      CC_DC,      CC_DC,     CC_DC,             0,         CC_DC,     CC_DC,         CC_DC,             0,         0);
    cc.printClause(11, CC_DC,      0,          0,      CC_DC,     CC_DC,             0,         CC_DC,     CC_DC,             0,         CC_DC,         1);
    cc.printClause(11, CC_DC,      0,          1,      CC_DC,     CC_DC,             0,         CC_DC,     CC_DC,             1,         CC_DC,         1);
    cc.printClause(11, CC_DC,      0,          0,      CC_DC,     CC_DC,             1,         CC_DC,     CC_DC,             1,         CC_DC,         1);
    cc.printClause(11,     0,      0,      CC_DC,      CC_DC,     CC_DC,             1,         CC_DC,     CC_DC,         CC_DC,             1,         1);
    cc.printClause(11, CC_DC,  CC_DC,      CC_DC,          1,         0,             0,             0,         0,         CC_DC,             0,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,      CC_DC,          0,         1,             1,             0,         1,         CC_DC,             0,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,      CC_DC,          1,         1,             1,             1,         1,         CC_DC,             0,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,      CC_DC,          0,         0,             0,             0,         0,         CC_DC,             1,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,      CC_DC,          1,         0,             0,             1,         0,         CC_DC,             1,     CC_DC);
    cc.printClause(11, CC_DC,  CC_DC,      CC_DC,          0,         1,             1,             1,         1,         CC_DC,             1,     CC_DC);
    cc.printClause(11, CC_DC,      1,      CC_DC,          1,     CC_DC,             0,             0,     CC_DC,         CC_DC,             0,         0);
    cc.printClause(11, CC_DC,      1,      CC_DC,          0,     CC_DC,             0,             0,     CC_DC,         CC_DC,             1,         0);
    cc.printClause(11, CC_DC,      1,      CC_DC,          1,     CC_DC,             0,             1,     CC_DC,         CC_DC,             1,         0);
    cc.printClause(11, CC_DC,      0,      CC_DC,          0,     CC_DC,             1,             0,     CC_DC,         CC_DC,             0,         1);
    cc.printClause(11, CC_DC,      0,      CC_DC,          1,     CC_DC,             1,             1,     CC_DC,         CC_DC,             0,         1);
    cc.printClause(11, CC_DC,      0,      CC_DC,          0,     CC_DC,             1,             1,     CC_DC,         CC_DC,             1,         1);
}

MU_TEST_C(Add_Full_3::test) {
    for (unsigned a = 0; a < 8; a++) {
        for (unsigned b = 0; b < 8; b++) {
            for (unsigned c = 0; c < 2; c++) {
                SATSolver solver;
                solver.set_verbosity(0);
                solver.log_to_file("test.log");

                uint32_t ausgabe = a + b + c;

                solver_writeInt(solver, 0, 3, a);
                solver_writeInt(solver, 3, 3, b);
                solver_writeInt(solver, 6, 1, c);

                Add_Full_3 adder;
                fullCNF = true;
                adder.append(&solver);
                fullCNF = false;

                lbool ret = solver.solve();
                mu_assert(ret == l_True, "FullAdder UNSAT");

                mu_assert(((ausgabe >> 3) & 0x1) == solver_readInt(solver, 7, 1), "FullAdder Carry failed");
                mu_assert(((ausgabe >> 0) & 0x7) == solver_readInt(solver, 8, 3), "FullAdder Result failed");
            }
        }
    }
}
