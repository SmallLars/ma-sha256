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
#include "module/sha256.h"

#define THREAD_NUM 4
#define MAX_TEST 2

using std::vector;
using namespace CMSat;

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

    Sha256 sha256;
    sha256.create(&printer);
    sha256.create(&logger);

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
                    std::cout << ((s >> i) & 1 ? "" : "-") << v[i] << " ";
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
