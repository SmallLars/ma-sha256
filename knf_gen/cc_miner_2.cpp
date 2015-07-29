#include <assert.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>

#include "common/producer.h"

#include "cryptominisat4/cryptominisat.h"
#include "printer/solverprinter.h"
#include "printer/logger.h"
#include "module/add_prepare_32.h"
#include "module/shacore_ex1_32.h"

#define THREAD_NUM 4
#define MAX_TEST 2

using std::vector;
using namespace CMSat;

static uint32_t sha_k[64] = {\
     0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,\
     0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,\
     0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,\
     0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,\
     0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,\
     0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,\
     0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,\
     0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2};

void signalHandler(int signum) {
    exit(1);
}

void *calculate(void* producer) {
    SolverConf config;
    config.doSQL = false;
    SATSolver solver(config);
//    solver.log_to_file("solver.log");
    Logger logger("sha256.log");
    SolverPrinter printer(&solver);

    unsigned global_input[64];
    for (unsigned i = 0; i < 16; i++) global_input[i] = i * 32;

    unsigned vars[8];
    for (unsigned i = 0; i < 8; i++) vars[i] = 512 + i * 32;

    unsigned varCount = 768;

    Add_Prepare_32 adder;
    ShaCore_Ex1_32 core(0);

    for (unsigned i = 0; i < 64; i++) {
        vector<unsigned> subinputs;
        for (unsigned n = 0; n < 8; n++) subinputs.push_back(vars[n]);

        if (i < 16) {
            subinputs.push_back(global_input[i]);
        } else {
            vector<unsigned> prepareinputs;
            prepareinputs.push_back(global_input[i - 16]);
            prepareinputs.push_back(global_input[i - 15]);
            prepareinputs.push_back(global_input[i -  7]);
            prepareinputs.push_back(global_input[i -  2]);
            adder.setInputs(prepareinputs);
            adder.setStart(varCount);
            adder.create(&printer);
            adder.create(&logger);
            varCount += adder.getAdditionalVarCount();

            global_input[i] = adder.getOutput();
            subinputs.push_back(adder.getOutput());
        }

        core.setValue(sha_k[i]);
        core.setInputs(subinputs);
        core.setStart(varCount);
        core.create(&printer);
        core.create(&logger);
        varCount += core.getAdditionalVarCount();

        for (unsigned n = 7; n > 0; n--) vars[n] = vars[n - 1];
        vars[0] = core.getOutput();
        vars[4] = core.getOutput() + 32;
    }

    vector<unsigned> v;
    while (((Producer*) producer)->getWork(v)) {
        for (unsigned s = 0; s < (1u << v.size()); s++) {
            vector<Lit> assumptions;
            for (unsigned i = 0; i < v.size(); i++) {
                assumptions.push_back(Lit(v[i], (s >> i) & 1));
            }

            lbool ret = solver.solve(&assumptions);
            if (ret == l_False) {
                std::cout << " Hurra: ";
                for (unsigned i = 0; i < v.size(); i++) {
                    std::cout << ((s >> i) & 1 ? "-" : "") << v[i] << " ";
                }
                std::cout << "\n";
            }
        }
    }
    pthread_exit(NULL);
}

int main() {
    signal(SIGINT, signalHandler);

    static Producer producer(MAX_TEST);
    for (unsigned i = 0; i < 64; i++) producer.addVar(i);

    pthread_t threads[THREAD_NUM];
    for (unsigned i = 0; i < THREAD_NUM; i++) {
        int rc = pthread_create(threads + i, NULL, calculate, (void*) &producer);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }

    for (unsigned i = 0; i < THREAD_NUM; i++) {
        pthread_join(threads[i], NULL);
    }

    return EXIT_SUCCESS;
}
