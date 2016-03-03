#include "maj_32.h"

#include "clausecreator.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Maj_32::stats[STATS_LENGTH];

// MAJ(a; b; c) = (a AND b) XOR (a AND c) XOR (b AND c)
// (!r | a | b) & (!r | a | c) & (!r | b | c) & (r | !a | !b) & (r | !a | !c) & (r | !b | !c)

// a =  0 -  31
// b = 32 -  63
// c = 64 -  95
// r = 96 - 127

Maj_32::Maj_32() : Modul(32, 3, 1) {
    output = 96;
}

Maj_32::~Maj_32() {
}

unsigned* Maj_32::getStats() {
    return stats;
}

void Maj_32::create(Collector* collector) {
    collector->newModul(10, "Maj_32", this);

    ClauseCreator cc(collector);

    for (unsigned i = 0; i < 32; i++) {
        //                     r_out           a_in           b_in           c_in
        cc.setLiterals(4, output + i, inputs[0] + i, inputs[1] + i, inputs[2] + i);
        cc.printClause(4,          1,             0,             0,         CC_DC);
        cc.printClause(4,          0,             1,             1,         CC_DC);
        cc.printClause(4,          1,             0,         CC_DC,             0);
        cc.printClause(4,          1,         CC_DC,             0,             0);
        cc.printClause(4,          0,             1,         CC_DC,             1);
        cc.printClause(4,          0,         CC_DC,             1,             1);
    }
}

MU_TEST_C(Maj_32::test) {
    unsigned a[] = {0xabcdef98};
    unsigned b[] = {0x651d8fa1};
    unsigned c[] = {0x456af012};

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.set_verbosity(0);
        solver.log_to_file("test.log");

        uint32_t ausgabe = (a[t] & b[t]) ^ (a[t] & c[t]) ^ (b[t] & c[t]);

        solver_writeInt(solver,  0, 32, a[t]);
        solver_writeInt(solver, 32, 32, b[t]);
        solver_writeInt(solver, 64, 32, c[t]);

        Maj_32 maj;
        maj.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "MAJ UNSAT");
        mu_assert(ausgabe == solver_readInt(solver, 96, 32), "MAJ failed");
    }
}
