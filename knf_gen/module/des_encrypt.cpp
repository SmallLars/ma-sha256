#include "des_encrypt.h"

#include "des_round.h"

#include  <algorithm>

#include "../common/solvertools.h"

using std::vector;
using namespace CMSat;

unsigned Des_Encrypt::stats[STATS_LENGTH];

Des_Encrypt::Des_Encrypt() : Modul(64, 2, 1) {
    setInputsBitWidth(64, 56); //plaintext, k

    Des_Round des_round(1);
    output = start + des_round.getAdditionalVarCount() * 16;// - 64;
}

Des_Encrypt::~Des_Encrypt() {
}

unsigned* Des_Encrypt::getStats() {
    return stats;
}

void Des_Encrypt::create(Printer* printer) {
    printer->newModul(30, "Des_Encrypt", this);

    unsigned l = inputs[0];
    unsigned r = inputs[0] + 32;
    unsigned additional_vars = 0;

    for(int current_round = 0; current_round < 16; current_round++){
        vector<unsigned> subinputs;
        subinputs.push_back(l);
        subinputs.push_back(r);
        subinputs.push_back(inputs[1]);

        Des_Round des_round(current_round);
        des_round.setInputs(subinputs);
        des_round.setStart(start + additional_vars);
/*
        if (current_round >= 14){
            des_round.setOutput(output + (current_round - 14) * 32);
        }
*/
//        if (current_round >= 14) printf("%u: %u\n", current_round, des_round.getOutput());

        des_round.create(printer);

        additional_vars += des_round.getAdditionalVarCount();
//        if (current_round >= 14){
//            additional_vars -= 32;
//        }

        l = r;
        r = des_round.getOutput();
    }



}

MU_TEST_C(Des_Encrypt::test) {
    uint64_t input[] = {0x577b5a24e23b3f33, 0x874ac85243840ef2, 0xae0fd81a8c7e1ac8, 0x4f6bf8188ab7be8d, 0x790527c605332ab5};
    uint64_t k[] = {0x4573f91f9cda87, 0xd449a7db22d748, 0x5dc2a16228f0cc, 0x6a65b629ac38ad, 0x35b981a78961ba};
    uint64_t out[] = {0x3ddb2d3e3134c0fa, 0x5c3729e67909bf09, 0xad46290c7de517f2L, 0x14b02ea8f7aeb4d9, 0x2560817f81b8dfa6};

    for (unsigned t = 0; t < 5; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");

        solver_writeInt_msb(solver,  0, 64, input[t]);
        solver_writeInt_msb(solver, 64, 56, k[t]);

        Des_Encrypt des_encrypt;
        des_encrypt.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "DES_Encrypt UNSAT");

        printf("\n");
        printf("Erwartet: %08lx\n", out[t]);
        printf("Erhalten: %08lx\n", solver_readInt_msb(solver, des_encrypt.getOutput(), 64));

        printf("L: %08lx\n", solver_readInt_msb(solver, 1768, 32));
        printf("R: %08lx\n", solver_readInt_msb(solver, 1880, 32));

        mu_assert(out[t] == solver_readInt_msb(solver, des_encrypt.getOutput(), 64), "DES_Encrypt failed");
    }
}
