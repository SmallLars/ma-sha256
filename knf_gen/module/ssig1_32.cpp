#include "ssig1_32.h"

#include "const.h"

using namespace CMSat;

unsigned Ssig1_32::stats[STATS_LENGTH];

Ssig1_32::Ssig1_32() : Modul(32, 1, 1) {
    inputs.push_back(0);
    output = 32;
}

Ssig1_32::~Ssig1_32() {
}

unsigned* Ssig1_32::getStats() {
    return stats;
}

void Ssig1_32::create(Printer* printer) {
    for (unsigned i = 0; i < 22; i++) {
        createXOR(printer,
                  output + i, 
                  inputs[0] + ((i + 17) % 32),
                  inputs[0] + ((i + 19) % 32),
                  inputs[0] +   i + 10
        );
    }
    for (unsigned i = 22; i < 32; i++) {
        createXOR(printer,
                  output + i, 
                  inputs[0] + ((i + 17) % 32),
                  inputs[0] + ((i + 19) % 32)
        );
    }
}

MU_TEST_C(Ssig1_32::test) {
    unsigned a[] = {0xabcdef98};

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");
        solver.set_num_threads(4);

        uint32_t ausgabe = (a[t] >> 17 | a[t] << (32-17)) ^ (a[t] >> 19 | a[t] << (32-19)) ^ (a[t] >> 10);
        uint32_t result = 0;

        Const con(32, a[t]);
        con.append(&solver);

        Ssig1_32 ssig1;
        ssig1.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "SSIG1 UNSAT");

        for (unsigned i = 63; i >=32; i--) {
            result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 32));
        }

        mu_assert(ausgabe == result, "SSIG1 failed");
    }
}
