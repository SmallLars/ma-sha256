#include "des_encrypt.h"

#include "des_round.h"

#include  <algorithm>

#include "../common/solvertools.h"

using std::vector;
using namespace CMSat;

unsigned Des_Encrypt::stats[STATS_LENGTH];

Des_Encrypt::Des_Encrypt() : Modul(32, 2, 1) {
    Des_Round des_round(1);
    setInputsBitWidth(64, 56); //plaintext, k
    output = start + des_round.getAdditionalVarCount()*16 - 64; //TODO eventuell 64 abziehen
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
        Des_Round des_round(current_round);
        vector<unsigned> subinputs;
        subinputs.push_back(l);
        subinputs.push_back(r);
        subinputs.push_back(inputs[1]);

        des_round.setInputs(subinputs);
        des_round.setStart(start + additional_vars);
        if (current_round == 14 || current_round == 15){
            des_round.setOutput(output + (current_round - 14) * 32);
        }
        des_round.create(printer);

        additional_vars += des_round.getAdditionalVarCount();
        if (current_round == 14 || current_round == 15){
            additional_vars -= 32;
        }

        l = r;
        r = des_round.getOutput();
    }



}

MU_TEST_C(Des_Encrypt::test) {
    uint64_t input[] = {0x5f4fa737c38bc47d, 0xfe099c64b371af98, 0x1b52740ffd6225a5, 0xc5600e0cb0f85e79, 0x51770b0705a373db};
    uint64_t k[] = {0x6c79ac0d445e39, 0xa067b5d99a49b7, 0x3319690ad91dff, 0xb010c07e36c3fd, 0xf823d812338acf};
    uint64_t out[] = {0xe73899b620788117, 0x6f6285a606c902ca, 0xc0f1ececf6a017a0, 0xb2605cb8f1c629e9L, 0xbfb32c5460589416};

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

        mu_assert(out[t] == solver_readInt_msb(solver, des_encrypt.getOutput(), 64), "DES_Encrypt failed");
    }
}
