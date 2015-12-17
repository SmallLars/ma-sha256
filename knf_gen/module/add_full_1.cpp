#include "add_full_1.h"

#include "clausecreator.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Add_Full_1::stats[STATS_LENGTH];

Add_Full_1::Add_Full_1() : Modul(1, 3, 1) {
    output = 4;
}

Add_Full_1::~Add_Full_1() {
}

unsigned* Add_Full_1::getStats() {
    return stats;
}

void Add_Full_1::create(Printer* printer) {
    printer->newModul(0, "Add_Full_1", this);

    ClauseCreator cc(printer);
    //                c_out   s_out       a_in       b_in       c_in
    cc.setLiterals(5, start, output, inputs[0], inputs[1], inputs[2]);

#ifdef XOR_SUPPORT
    //                    4       5          1          2          3
    cc.printClause(5,     1,  CC_DC,         0,         0,     CC_DC);
    cc.printClause(5,     0,  CC_DC,         1,         1,     CC_DC);
    cc.printClause(5,     1,  CC_DC,         0,     CC_DC,         0);
    cc.printClause(5,     1,  CC_DC,     CC_DC,         0,         0);
    cc.printClause(5,     0,  CC_DC,         1,     CC_DC,         1);
    cc.printClause(5,     0,  CC_DC,     CC_DC,         1,         1);

    // XOR ->          !s_out       a_in       b_in       c_in
    createXOR(printer, output, inputs[0], inputs[1], inputs[2]);

    #ifdef ADDITIONAL_CLAUSES
        //                    4       5          1          2          3
        cc.printClause(5,     0,      0,         1,     CC_DC,     CC_DC);
        cc.printClause(5,     1,      1,         0,     CC_DC,     CC_DC);
        cc.printClause(5,     0,      0,     CC_DC,         1,     CC_DC);
        cc.printClause(5,     1,      1,     CC_DC,         0,     CC_DC);
        cc.printClause(5,     0,      0,     CC_DC,     CC_DC,         1);
        cc.printClause(5,     1,      1,     CC_DC,     CC_DC,         0);
        //XXX cc.printClause(5,     0,  CC_DC,         1,         1,         0);
        //XXX cc.printClause(5,     0,  CC_DC,         1,         0,         1);
        //XXX cc.printClause(5,     1,  CC_DC,         1,         0,         0);
        //XXX cc.printClause(5,     0,  CC_DC,         0,         1,         1);
        //XXX cc.printClause(5,     1,  CC_DC,         0,         1,         0);
        //XXX cc.printClause(5,     1,  CC_DC,         0,         0,         1);
    #endif
#else
    //                    4       5          1          2          3
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

    #ifdef ADDITIONAL_CLAUSES
        //                    4       5          1          2          3
        cc.printClause(5,     0,  CC_DC,         1,     CC_DC,         1);
        cc.printClause(5,     1,  CC_DC,         0,     CC_DC,         0);
        cc.printClause(5,     0,      0,         1,     CC_DC,     CC_DC);
        cc.printClause(5,     1,      1,         0,     CC_DC,     CC_DC);
        cc.printClause(5,     0,  CC_DC,     CC_DC,         1,         1);
        cc.printClause(5,     1,  CC_DC,     CC_DC,         0,         0);
        cc.printClause(5,     0,      0,     CC_DC,         1,     CC_DC);
        cc.printClause(5,     1,      1,     CC_DC,         0,     CC_DC);
        //XXX cc.printClause(5,     0,  CC_DC,         1,         1,         0);
        //XXX cc.printClause(5,     0,  CC_DC,         1,         0,         1);
        //XXX cc.printClause(5,     1,  CC_DC,         1,         0,         0);
        //XXX cc.printClause(5,     0,  CC_DC,         0,         1,         1);
        //XXX cc.printClause(5,     1,  CC_DC,         0,         1,         0);
        //XXX cc.printClause(5,     1,  CC_DC,         0,         0,         1);
        cc.printClause(5, CC_DC,      1,         1,         1,         0);
        cc.printClause(5, CC_DC,      0,         0,         0,         1);
    #endif
#endif
}

MU_TEST_C(Add_Full_1::test) {
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

                Add_Full_1 adder;
                adder.append(&solver);

                lbool ret = solver.solve();
                mu_assert(ret == l_True, "FullAdder UNSAT");
                mu_assert(((ausgabe >> 1) & 0x1) == solver_readInt(solver, 3, 1), "FullAdder Carry failed");
                mu_assert(((ausgabe >> 0) & 0x1) == solver_readInt(solver, 4, 1), "FullAdder Result failed");
            }
        }
    }
}
