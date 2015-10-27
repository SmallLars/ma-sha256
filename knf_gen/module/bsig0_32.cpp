#include "bsig0_32.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Bsig0_32::stats[STATS_LENGTH];

Bsig0_32::Bsig0_32() : Modul(32, 1, 1) {
    output = 32;
}

Bsig0_32::~Bsig0_32() {
}

unsigned* Bsig0_32::getStats() {
    return stats;
}

void Bsig0_32::create(Printer* printer) {
    printer->newModul(10, "Bsig0_32", this);

    for (unsigned i = 0; i < 32; i++) {
        createXOR(printer,
                  output + i, 
                  inputs[0] + ((i +  2) % 32),
                  inputs[0] + ((i + 13) % 32),
                  inputs[0] + ((i + 22) % 32)
        );
    }
}

MU_TEST_C(Bsig0_32::test) {
    unsigned a[] = {0xabcdef98};

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");

        uint32_t ausgabe = (a[t] >> 2 | a[t] << (32-2)) ^ (a[t] >> 13 | a[t] << (32-13)) ^ (a[t] >> 22 | a[t] << (32-22));

        solver_writeInt(solver, 0, 32, a[t]);

        Bsig0_32 bsig0;
        bsig0.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "BSIG0 UNSAT");
        mu_assert(ausgabe == solver_readInt(solver, 32, 32), "BSIG0 failed");
    }
}
