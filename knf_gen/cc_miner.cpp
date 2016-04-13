#include <assert.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>

#include "common/producer.h"
#include "common/clausetools.h"

#include "cryptominisat4/cryptominisat.h"
#include "module/const.h"
#include "module/bsig0_32.h"
#include "module/maj_32.h"
#include "module/add_32.h"
#include "module/constadd_32.h"
#include "module/ssig0_32.h"
#include "module/add_b0maj_32.h"
#include "module/add_b1ch_32.h"
#include "module/add_ssig_32.h"
#include "module/prepare_32.h"

#define THREAD_NUM 4
#define MAX_TEST 2

#define MODUL Add_32
//#define MODUL Add_B0Maj_32
//#define MODUL Add_B1Ch_32
//#define MODUL Add_Ssig_32
//#define MODUL Prepare_32

using std::vector;
using namespace CMSat;

void signalHandler(int signum) {
    exit(1);
}

void *calculate(void* producer) {
    SATSolver solver;
//    solver.log_to_file("solver.log");

    MODUL modul;
    modul.append(&solver);

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
                printClause(std::cout, assumptions, true);
            }
        }
    }
    pthread_exit(NULL);
}

int main() {
    signal(SIGINT, signalHandler);

    MODUL modul;
//    unsigned out = modul.getOutput();

    static Producer producer(MAX_TEST);
    for (unsigned i = 0; i < modul.getVarCount(); i++) producer.addVar(i);
//    for (unsigned i = 0; i < modul.getInputNum(); i++) producer.addVar(i);
//    producer.setOutStart();
//    for (unsigned i = out; i < out + modul.getOutputNum(); i++) producer.addVar(i);

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
