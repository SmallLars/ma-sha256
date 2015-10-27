#include "add_full_2.h"

#include "clausecreator.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Add_Full_2::stats[STATS_LENGTH];

Add_Full_2::Add_Full_2() : Modul(2, 3, 1) {
    inputs.push_back(0);
    inputs.push_back(2);
    inputs.push_back(4);
    start = 5;
    output = 6;
}

Add_Full_2::~Add_Full_2() {
}

unsigned* Add_Full_2::getStats() {
    return stats;
}

void Add_Full_2::create(Printer* printer) {
    printer->newModul(1, "Add_Full_2", this);

    ClauseCreator cc(printer);
    //                c_out  s_out[0]  s_out[1]    a_in[0]        a_in[1]    b_in[0]        b_in[1]       c_in
    cc.setLiterals(8, start, output, output + 1, inputs[0], inputs[0] + 1, inputs[1], inputs[1] + 1, inputs[2]);
    cc.printClause(8, CC_DC,      0,      CC_DC,         1,         CC_DC,         1,         CC_DC,         1);
    cc.printClause(8, CC_DC,      1,      CC_DC,         0,         CC_DC,         0,         CC_DC,         0);
    cc.printClause(8,     0,  CC_DC,          0,     CC_DC,             1,     CC_DC,         CC_DC,     CC_DC);
    cc.printClause(8,     1,  CC_DC,          1,     CC_DC,             0,     CC_DC,         CC_DC,     CC_DC);
    cc.printClause(8,     0,  CC_DC,      CC_DC,         1,         CC_DC,         1,             1,     CC_DC);
    cc.printClause(8,     1,  CC_DC,      CC_DC,         0,         CC_DC,         0,             0,     CC_DC);
    cc.printClause(8, CC_DC,      1,      CC_DC,         0,         CC_DC,         1,         CC_DC,         1);
    cc.printClause(8, CC_DC,      1,      CC_DC,         1,         CC_DC,         0,         CC_DC,         1);
    cc.printClause(8,     0,      0,      CC_DC,     CC_DC,         CC_DC,     CC_DC,             1,         1);
    cc.printClause(8, CC_DC,      0,      CC_DC,         0,         CC_DC,         1,         CC_DC,         0);
    cc.printClause(8, CC_DC,      0,      CC_DC,         1,         CC_DC,         0,         CC_DC,         0);
    cc.printClause(8,     1,      1,      CC_DC,     CC_DC,         CC_DC,     CC_DC,             0,         0);
    cc.printClause(8, CC_DC,  CC_DC,          0,         1,             1,         1,             1,     CC_DC);
    cc.printClause(8, CC_DC,  CC_DC,          1,         0,             1,         0,             1,     CC_DC);
    cc.printClause(8, CC_DC,  CC_DC,          0,         0,             0,         0,             1,     CC_DC);
    cc.printClause(8, CC_DC,  CC_DC,          1,         1,             1,         1,             0,     CC_DC);
    cc.printClause(8, CC_DC,  CC_DC,          0,         1,             0,         1,             0,     CC_DC);
    cc.printClause(8, CC_DC,  CC_DC,          1,         0,             0,         0,             0,     CC_DC);
    cc.printClause(8, CC_DC,      0,          0,     CC_DC,             1,     CC_DC,             1,         1);
    cc.printClause(8, CC_DC,      0,          1,     CC_DC,             1,     CC_DC,             0,         1);
    cc.printClause(8, CC_DC,      0,          0,     CC_DC,             0,     CC_DC,             0,         1);
    cc.printClause(8, CC_DC,      1,          1,     CC_DC,             1,     CC_DC,             1,         0);
    cc.printClause(8, CC_DC,      1,          0,     CC_DC,             0,     CC_DC,             1,         0);
    cc.printClause(8, CC_DC,      1,          1,     CC_DC,             0,     CC_DC,             0,         0);

/*
   4  5 -6 -7 0

   4 -5  6 7 0
-2 4 -5  6 7 0

2 -4 -5  6 7 0
2 -4 -5    7 -8 0
*/

#ifdef ADDITIONAL_CLAUSES
/*
    ClauseCreator cc(printer);
    //                  6       7        8          1                 2        3             4             5
    //                c_out  s_out[0]  s_out[1]    a_in[0]        a_in[1]    b_in[0]        b_in[1]       c_in
    cc.setLiterals(8, start, output, output + 1, inputs[0], inputs[0] + 1, inputs[1], inputs[1] + 1, inputs[2]);
    // 2 5 -6 -7 0
    cc.printClause(8,     0,      0,      CC_DC,     CC_DC,             1,     CC_DC,         CC_DC,         1);
    // 2 -5 6 7 0
    cc.printClause(8,     1,      1,      CC_DC,     CC_DC,             1,     CC_DC,         CC_DC,         0);
*/
#endif
}

MU_TEST_C(Add_Full_2::test) {
    unsigned a[] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};
    unsigned b[] = {0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3};
    unsigned c[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    for (unsigned t = 0; t < 32; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");

        uint32_t ausgabe = a[t] + b[t] + c[t];

        solver_writeInt(solver, 0, 2, a[t]);
        solver_writeInt(solver, 2, 2, b[t]);
        solver_writeInt(solver, 4, 1, c[t]);

        Add_Full_2 adder;
        adder.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "FullAdder UNSAT");
        mu_assert(((ausgabe >> 2) & 0x1) == solver_readInt(solver, 5, 1), "FullAdder Carry failed");
        mu_assert(((ausgabe >> 0) & 0x3) == solver_readInt(solver, 6, 2), "FullAdder Result failed");
    }
}