#include "ssig0_32.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Ssig0_32::stats[STATS_LENGTH];

Ssig0_32::Ssig0_32() : Modul(32, 1, 1) {
    inputs.push_back(0);
    output = 32;
}

Ssig0_32::~Ssig0_32() {
}

unsigned* Ssig0_32::getStats() {
    return stats;
}

void Ssig0_32::create(Printer* printer) {
    printer->newModul("Ssig0_32", this);

    for (unsigned i = 0; i < 29; i++) {
        createXOR(printer,
                  output + i, 
                  inputs[0] + ((i +  7) % 32),
                  inputs[0] + ((i + 18) % 32),
                  inputs[0] +   i +  3
        );
    }
    for (unsigned i = 29; i < 32; i++) {
        createXOR(printer,
                  output + i, 
                  inputs[0] + ((i +  7) % 32),
                  inputs[0] + ((i + 18) % 32)
        );
    }
}

MU_TEST_C(Ssig0_32::test) {
    unsigned a[] = {0xabcdef98};

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");
        solver.set_num_threads(4);

        uint32_t ausgabe = (a[t] >> 7 | a[t] << (32-7)) ^ (a[t] >> 18 | a[t] << (32-18)) ^ (a[t] >> 3);

        solver_writeInt(solver, 0, 32, a[t]);

        Ssig0_32 ssig0;
        ssig0.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "SSIG0 UNSAT");
        mu_assert(ausgabe == solver_readInt(solver, 32, 32), "SSIG0 failed");
    }
}
