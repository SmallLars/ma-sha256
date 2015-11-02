#include "add_last_4.h"

#include "clausecreator.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Add_Last_4::stats[STATS_LENGTH];

Add_Last_4::Add_Last_4() : Modul(4, 3, 1) {
    setInputsBitWidth(4, 4, 1);
    output = 9;
}

Add_Last_4::~Add_Last_4() {
}

unsigned* Add_Last_4::getStats() {
    return stats;
}

void Add_Last_4::create(Printer* printer) {
    printer->newModul(3, "Add_Last_4", this);

    ClauseCreator cc(printer);
    //               s_out[0]    s_out[1]    s_out[2]    s_out[3]    a_in[0]        a_in[1]        a_in[2]        a_in[3]    b_in[0]        b_in[1]        b_in[2]        b_in[3]       c_in
    cc.setLiterals(13, output, output + 1, output + 2, output + 3, inputs[0], inputs[0] + 1, inputs[0] + 2, inputs[0] + 3, inputs[1], inputs[1] + 1, inputs[1] + 2, inputs[1] + 3, inputs[2]);
//    cc.printClause(13, CC_DC,      1,      0 ...........);

    cc.setLiterals(2, output, output);
    cc.printClause(2,      0,      1);

#ifdef ADDITIONAL_CLAUSES
    //                  10          11          12          13          1              2              3              4          5              6              7              8            9
    //               s_out[0]    s_out[1]    s_out[2]    s_out[3]    a_in[0]        a_in[1]        a_in[2]        a_in[3]    b_in[0]        b_in[1]        b_in[2]        b_in[3]       c_in

#endif
}

MU_TEST_C(Add_Last_4::test) {
    for (unsigned a = 0; a < 16; a++) {
        for (unsigned b = 0; b < 16; b++) {
            for (unsigned c = 0; c < 2; c++) {
                SATSolver solver;
                solver.log_to_file("test.log");

                uint32_t ausgabe = a + b + c;

                solver_writeInt(solver, 0, 4, a);
                solver_writeInt(solver, 4, 4, b);
                solver_writeInt(solver, 8, 1, c);

                Add_Last_4 adder;
                adder.append(&solver);

                lbool ret = solver.solve();
                mu_assert(ret == l_True, "LastAdder UNSAT");
                mu_assert(((ausgabe >> 0) & 0x15) == solver_readInt(solver, 9, 4), "LastAdder Result failed");
            }
        }
    }
}
