#include <assert.h>
#include <vector>
#include <stdio.h>
#include <pthread.h>

#include "producer.h"

#include "cryptominisat4/cryptominisat.h"
#include "module/const.h"
#include "module/bsig0_32.h"
#include "module/maj_32.h"
#include "module/adder_32.h"
#include "module/constadder_32.h"
#include "module/ssig0_32.h"
#include "module/adder_ssig_32.h"
#include "module/adder_prepare_32.h"

#define THREAD_NUM 4
#define MODUL Adder_Ssig_32

using std::vector;
using namespace CMSat;

void *calculate(void* producer) {
    SolverConf config;
    config.doSQL = false;

    vector<unsigned> v;
    while (((Producer*) producer)->getWork(v)) {

        for (unsigned s = 0; s < (1u << v.size()); s++) {
            SATSolver solver(config);
//            solver.log_to_file("solver.log");

            MODUL modul;
            modul.append(&solver);

            vector<Lit> clause(1);
            for (unsigned i = 0; i < v.size(); i++) {
                clause[0] = Lit(v[i], (s >> i) & 1);
                solver.add_clause(clause);
            }

            lbool ret = solver.solve();
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
    MODUL modul;
    unsigned out = modul.getOutput();

    Producer producer(3);
    for (unsigned i = 0; i < modul.getInputNum(); i++) producer.addVar(i);
    for (unsigned i = out; i < out + modul.getOutputNum(); i++) producer.addVar(i);

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

    return 0;
}
