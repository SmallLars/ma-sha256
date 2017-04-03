#include "add_half_1.h"

#include <stdio.h>
#include <fstream>
#include <stdlib.h>

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

void Add_Half_1::create(Collector* collector) {
    collector->newModul(0, "Add_Half_1", this);

    ClauseCreator cc(collector);
    //                c_out   s_out       a_in       b_in
    cc.setLiterals(4, start, output, inputs[0], inputs[1]);

#ifdef XOR_SUPPORT
    // AND ->            c_out   =   a_in   &   b_in
    createAND(collector, start, inputs[0], inputs[1]);

    // XOR ->             s_out   =   a_in   ^   b_in
    createXOR(collector, output, inputs[0], inputs[1]);

    #ifdef ADDITIONAL_CLAUSES
        //                    3       4          1          2
        cc.printClause(4,     0,      0,     CC_DC,     CC_DC);
        cc.printClause(4,     1,      1,         0,     CC_DC);
        cc.printClause(4,     1,      1,     CC_DC,         0);
    #endif
#else
    //                    3       4          1          2
    cc.printClause(4, CC_DC,      0,         1,         1);
    cc.printClause(4,     0,      0,     CC_DC,     CC_DC);
    cc.printClause(4,     0,  CC_DC,     CC_DC,         1);
    cc.printClause(4,     1,  CC_DC,         0,         0);
    cc.printClause(4, CC_DC,      1,         1,         0);
    cc.printClause(4, CC_DC,      1,         0,         1);

    #ifdef ADDITIONAL_CLAUSES
        //                    3       4          1          2
        cc.printClause(4,     0,  CC_DC,         1,     CC_DC);
        cc.printClause(4, CC_DC,      0,         0,         0);
        cc.printClause(4,     1,      1,         0,     CC_DC);
        cc.printClause(4,     1,      1,     CC_DC,         0);
    #endif
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

            adder.writeDimacs("add_half_1.dimacs");
            std::ofstream outputFile("add_half_1.dimacs", std::ofstream::app);
            if (a == 0) outputFile << "-"; outputFile << "1 0\n";
            if (b == 0) outputFile << "-"; outputFile << "2 0\n";
            if (solver_readInt(solver, 2, 1) == 0) outputFile << "-"; outputFile << "3 0\n";
            if (solver_readInt(solver, 3, 1) == 0) outputFile << "-"; outputFile << "4 0\n";
            outputFile.close();
            int status = system("./cryptominisat/cryptominisat4 add_half_1.dimacs > /dev/null");
            remove("add_half_1.dimacs");
            mu_assert(WEXITSTATUS(status) == 10, "HalfAdder wrong DIMACS");
        }
    }
}
