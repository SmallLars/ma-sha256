#include "ch_32.h"

#include "clausecreator.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Ch_32::stats[STATS_LENGTH];

// CH(a; b; c) = (a AND b) XOR ( (NOT a) AND c)
// (!r | !a | b) & (!r | a | c) & (r |!a | !b) & (r | a | !c)

// a =  0 -  31
// b = 32 -  63
// c = 64 -  95
// r = 96 - 127

Ch_32::Ch_32() : Modul(32, 3, 1) {
    inputs.push_back(0);
    inputs.push_back(32);
    inputs.push_back(64);
    output = 96;
}

Ch_32::~Ch_32() {
}

unsigned* Ch_32::getStats() {
    return stats;
}

void Ch_32::create(Printer* printer) {
    printer->newModul("Ch_32", this);

    ClauseCreator cc(printer);

    for (unsigned i = 0; i < 32; i++) {
        //                     r_out           a_in           b_in           c_in
        cc.setLiterals(4, output + i, inputs[0] + i, inputs[1] + i, inputs[2] + i);
        cc.printClause(4,          1,             0,             0,         CC_DC);
        cc.printClause(4,          0,             0,             1,         CC_DC);
        cc.printClause(4,          1,             1,         CC_DC,             0);
        cc.printClause(4,          0,             1,         CC_DC,             1);
    }
}

MU_TEST_C(Ch_32::test) {
    unsigned a[] = {0xabcdef98};
    unsigned b[] = {0x651d8fa1};
    unsigned c[] = {0x456af012};

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");

        uint32_t ausgabe = (a[t] & b[t]) ^ ((~a[t]) & c[t]);

        solver_writeInt(solver,  0, 32, a[t]);
        solver_writeInt(solver, 32, 32, b[t]);
        solver_writeInt(solver, 64, 32, c[t]);

        Ch_32 ch;
        ch.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "CH UNSAT");
        mu_assert(ausgabe == solver_readInt(solver, 96, 32), "CH failed");
    }
}
