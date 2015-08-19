#include "bsig1_32.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Bsig1_32::stats[STATS_LENGTH];

Bsig1_32::Bsig1_32() : Modul(32, 1, 1) {
    inputs.push_back(0);
    output = 32;
}

Bsig1_32::~Bsig1_32() {
}

unsigned* Bsig1_32::getStats() {
    return stats;
}

void Bsig1_32::create(Printer* printer) {
    printer->newModul("Bsig1_32", this);

    for (unsigned i = 0; i < 32; i++) {
        createXOR(printer,
                  output + i, 
                  inputs[0] + ((i +  6) % 32),
                  inputs[0] + ((i + 11) % 32),
                  inputs[0] + ((i + 25) % 32)
        );
    }
}

MU_TEST_C(Bsig1_32::test) {
    unsigned a[] = {0xabcdef98};

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");
        solver.set_num_threads(4);

        uint32_t ausgabe = (a[t] >> 6 | a[t] << (32-6)) ^ (a[t] >> 11 | a[t] << (32-11)) ^ (a[t] >> 25 | a[t] << (32-25));

        solver_writeInt(solver, 0, 32, a[t]);

        Bsig1_32 bsig1;
        bsig1.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "BSIG1 UNSAT");
        mu_assert(ausgabe == solver_readInt(solver, 32, 32), "BSIG1 failed");
    }
}
