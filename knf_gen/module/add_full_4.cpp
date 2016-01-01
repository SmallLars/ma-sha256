#include "add_full_4.h"

#include "clausecreator.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Add_Full_4::stats[STATS_LENGTH];
static bool fullCNF = false;

Add_Full_4::Add_Full_4() : Modul(3, 3, 1) {
    setInputsBitWidth(4, 4, 1);
    output = 10;
}

Add_Full_4::~Add_Full_4() {
}

unsigned* Add_Full_4::getStats() {
    return stats;
}

void Add_Full_4::create(Printer* printer) {
    printer->newModul(3, "Add_Full_4", this);

    ClauseCreator cc(printer);
    //                 c_out  s_out[0]  s_out[1]    s_out[2]    s_out[3]
    cc.setLiterals(5, start, output, output + 1, output + 2, output + 3);
    //                  a_in[0]        a_in[1]        a_in[2]        a_in[3]
    cc.addLiterals(4, inputs[0], inputs[0] + 1, inputs[0] + 2, inputs[0] + 3);
    //                  b_in[0]        b_in[1]        b_in[2]        b_in[3]       c_in
    cc.addLiterals(5, inputs[1], inputs[1] + 1, inputs[1] + 2, inputs[1] + 3, inputs[2]);

    //                    10     11     12     13     14      1      2      3      4      5      6      7      8      9
    //XXX cc.printClause(14,     0, CC_DC, CC_DC, CC_DC,     0,     1,     1, CC_DC, CC_DC,     1, CC_DC, CC_DC,     1, CC_DC);
    //XXX cc.printClause(14,     0, CC_DC,     0, CC_DC,     0, CC_DC,     1, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC,     1,     1);

    if (!fullCNF) {
        cc.setLiterals(2, start, start);
        cc.printClause(2,     0,     1);
    }

    if (!fullCNF) return;

    cc.printClause(14, CC_DC,     0, CC_DC,     0,     0, CC_DC,     1, CC_DC,     1, CC_DC, CC_DC, CC_DC,     1,     1);
    cc.printClause(14, CC_DC, CC_DC, CC_DC,     0,     0,     1,     1, CC_DC,     1,     1, CC_DC, CC_DC,     1, CC_DC);
    cc.printClause(14, CC_DC,     1, CC_DC, CC_DC,     1, CC_DC,     0,     0,     0, CC_DC, CC_DC, CC_DC,     0,     0);
    cc.printClause(14, CC_DC, CC_DC, CC_DC, CC_DC,     1,     0,     0,     0,     0,     0, CC_DC, CC_DC,     0, CC_DC);
    cc.printClause(14,     0,     0, CC_DC,     0, CC_DC, CC_DC,     1, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC,     1,     1);
    cc.printClause(14,     0, CC_DC, CC_DC,     0, CC_DC,     1,     1, CC_DC, CC_DC,     1, CC_DC, CC_DC,     1, CC_DC);
    cc.printClause(14, CC_DC,     0, CC_DC,     0, CC_DC, CC_DC,     1,     0, CC_DC, CC_DC, CC_DC,     0, CC_DC,     1);
    cc.printClause(14, CC_DC, CC_DC, CC_DC,     0, CC_DC,     1,     1,     0, CC_DC,     1, CC_DC,     0, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC,     0,     0,     0, CC_DC, CC_DC, CC_DC,     1, CC_DC,     1, CC_DC,     1, CC_DC);
    cc.printClause(14, CC_DC,     1, CC_DC,     1,     0, CC_DC,     0, CC_DC,     1, CC_DC, CC_DC, CC_DC,     0,     0);
    cc.printClause(14, CC_DC, CC_DC, CC_DC,     1,     0,     0,     0, CC_DC,     1,     0, CC_DC, CC_DC,     0, CC_DC);
    cc.printClause(14, CC_DC, CC_DC,     1, CC_DC,     1, CC_DC, CC_DC,     0,     0, CC_DC,     0, CC_DC,     0, CC_DC);
    cc.printClause(14,     0, CC_DC,     0,     0, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC,     1, CC_DC,     1, CC_DC);
    cc.printClause(14,     1, CC_DC, CC_DC,     1,     1, CC_DC, CC_DC,     0, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC);
    cc.printClause(14,     1, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC,     0, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC,     0,     0, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC,     1,     0, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC,     1,     1,     0, CC_DC, CC_DC, CC_DC,     1, CC_DC,     0, CC_DC,     0, CC_DC);
    cc.printClause(14, CC_DC, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC,     1,     1, CC_DC, CC_DC,     1,     1, CC_DC);
    cc.printClause(14, CC_DC, CC_DC, CC_DC,     1,     1, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC,     0,     1, CC_DC);
    cc.printClause(14, CC_DC,     0, CC_DC,     1, CC_DC, CC_DC,     1,     1, CC_DC, CC_DC, CC_DC,     0, CC_DC,     1);
    cc.printClause(14, CC_DC,     1, CC_DC,     1, CC_DC, CC_DC,     0,     1, CC_DC, CC_DC, CC_DC,     1, CC_DC,     0);
    cc.printClause(14, CC_DC, CC_DC, CC_DC,     1, CC_DC,     0,     0,     1, CC_DC,     0, CC_DC,     1, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC, CC_DC,     1, CC_DC,     1,     1,     1, CC_DC,     1, CC_DC,     0, CC_DC, CC_DC);
    cc.printClause(14, CC_DC,     0, CC_DC,     0, CC_DC, CC_DC,     1,     1, CC_DC, CC_DC, CC_DC,     1, CC_DC,     1);
    cc.printClause(14, CC_DC,     0, CC_DC,     1, CC_DC, CC_DC,     1,     0, CC_DC, CC_DC, CC_DC,     1, CC_DC,     1);
    cc.printClause(14, CC_DC,     1, CC_DC,     0, CC_DC, CC_DC,     0,     1, CC_DC, CC_DC, CC_DC,     0, CC_DC,     0);
    cc.printClause(14, CC_DC,     1, CC_DC,     1, CC_DC, CC_DC,     0,     0, CC_DC, CC_DC, CC_DC,     0, CC_DC,     0);
    cc.printClause(14,     0, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC, CC_DC,     1,     1, CC_DC);
    cc.printClause(14, CC_DC, CC_DC, CC_DC,     0, CC_DC,     1,     1,     1, CC_DC,     1, CC_DC,     1, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC, CC_DC,     1, CC_DC,     1,     1,     0, CC_DC,     1, CC_DC,     1, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC, CC_DC,     0, CC_DC,     0,     0,     1, CC_DC,     0, CC_DC,     0, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC, CC_DC,     1, CC_DC,     0,     0,     0, CC_DC,     0, CC_DC,     0, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC, CC_DC,     1,     1, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC,     0,     0, CC_DC);
    cc.printClause(14, CC_DC,     1, CC_DC,     0, CC_DC, CC_DC,     0,     0, CC_DC, CC_DC, CC_DC,     1, CC_DC,     0);
    cc.printClause(14, CC_DC, CC_DC, CC_DC,     1,     0, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC,     0,     1, CC_DC);
    cc.printClause(14, CC_DC, CC_DC, CC_DC,     0, CC_DC,     0,     0,     0, CC_DC,     0, CC_DC,     1, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC, CC_DC,     1,     0, CC_DC, CC_DC,     0,     0, CC_DC, CC_DC, CC_DC,     1, CC_DC);
    cc.printClause(14,     0, CC_DC, CC_DC,     0,     0, CC_DC, CC_DC,     1, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC);
    cc.printClause(14,     0, CC_DC, CC_DC,     0, CC_DC, CC_DC, CC_DC,     1,     1, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC);
    cc.printClause(14,     0, CC_DC, CC_DC,     0,     0, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC);
    cc.printClause(14,     0, CC_DC, CC_DC,     0, CC_DC, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC,     1, CC_DC, CC_DC);
    cc.printClause(14, CC_DC,     0,     0, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC,     1);
    cc.printClause(14, CC_DC,     0,     1, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC,     1);
    cc.printClause(14, CC_DC,     0,     0, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC,     1);
    cc.printClause(14, CC_DC,     1,     1, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC,     0);
    cc.printClause(14, CC_DC,     1,     0, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC,     0);
    cc.printClause(14, CC_DC,     1,     1, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC,     0);
    cc.printClause(14, CC_DC, CC_DC,     0, CC_DC, CC_DC,     1,     1, CC_DC, CC_DC,     1,     1, CC_DC, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC,     1, CC_DC, CC_DC,     1,     0, CC_DC, CC_DC,     1,     1, CC_DC, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC,     1, CC_DC, CC_DC,     0,     1, CC_DC, CC_DC,     0,     1, CC_DC, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC,     0, CC_DC, CC_DC,     1,     0, CC_DC, CC_DC,     1,     0, CC_DC, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC,     0, CC_DC, CC_DC,     0,     1, CC_DC, CC_DC,     0,     0, CC_DC, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC,     1, CC_DC, CC_DC,     0,     0, CC_DC, CC_DC,     0,     0, CC_DC, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC,     1,     1, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC,     0,     1, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC,     0,     1, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC,     1,     0, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC,     0,     0, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC,     1,     1, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC,     0,     1, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC,     1,     1, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC,     1,     0, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC,     0,     0, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC,     1,     1, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC,     0,     0, CC_DC, CC_DC);
    cc.printClause(14,     0, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC,     1, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC,     0,     0, CC_DC, CC_DC,     0,     1, CC_DC);
    cc.printClause(14, CC_DC, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC,     0,     1, CC_DC, CC_DC,     0,     0, CC_DC);
    cc.printClause(14,     0, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC,     1,     1, CC_DC, CC_DC,     1, CC_DC, CC_DC);
    cc.printClause(14, CC_DC, CC_DC,     1,     0, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC,     0,     1, CC_DC, CC_DC);
    cc.printClause(14, CC_DC,     1, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC, CC_DC,     1);
    cc.printClause(14, CC_DC,     1, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC, CC_DC,     1);
    cc.printClause(14, CC_DC,     0, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC, CC_DC,     0);
    cc.printClause(14, CC_DC,     0, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC, CC_DC,     0);
    cc.printClause(14,     1, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC);
    cc.printClause(14,     1, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC,     0, CC_DC);
    cc.printClause(14,     1, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC, CC_DC,     0, CC_DC);
    cc.printClause(14,     0, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC, CC_DC,     1, CC_DC);
    cc.printClause(14, CC_DC,     0, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC, CC_DC,     1, CC_DC, CC_DC, CC_DC,     1);
    cc.printClause(14, CC_DC,     1, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC, CC_DC,     0, CC_DC, CC_DC, CC_DC,     0);
}

MU_TEST_C(Add_Full_4::test) {
    for (unsigned a = 0; a < 16; a++) {
        for (unsigned b = 0; b < 16; b++) {
            for (unsigned c = 0; c < 2; c++) {
                SATSolver solver;
                solver.set_verbosity(0);
                solver.log_to_file("test.log");

                uint32_t ausgabe = a + b + c;

                solver_writeInt(solver, 0, 4, a);
                solver_writeInt(solver, 4, 4, b);
                solver_writeInt(solver, 8, 1, c);

                Add_Full_4 adder;
                fullCNF = true;
                adder.append(&solver);
                fullCNF = false;

                lbool ret = solver.solve();
                mu_assert(ret == l_True, "FullAdder UNSAT");

                mu_assert(((ausgabe >> 4) & 0x1) == solver_readInt(solver, 9, 1), "FullAdder Carry failed");
                mu_assert(((ausgabe >> 0) & 0xF) == solver_readInt(solver, 10, 4), "FullAdder Result failed");
            }
        }
    }
}
