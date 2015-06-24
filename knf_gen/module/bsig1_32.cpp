#include "bsig1_32.h"

#include "const.h"

using namespace CMSat;

Bsig1_32::Bsig1_32() : Modul(32, 1, 1) {
    inputs.push_back(0);
    output = 32;
}

Bsig1_32::~Bsig1_32() {
}

void Bsig1_32::create(Printer* printer) {
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
        uint32_t result = 0;

        Const con(32, a[t]);
        con.append(&solver);

        Bsig1_32 bsig1;
        bsig1.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "BSIG1 UNSAT");

        for (unsigned i = 63; i >=32; i--) {
            result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 32));
        }

        mu_assert(ausgabe == result, "BSIG1 failed");
    }
}
