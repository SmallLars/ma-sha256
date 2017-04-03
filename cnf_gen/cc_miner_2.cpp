#include <assert.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>

#include "common/producer.h"
#include "common/clausetools.h"

#include "module/const.h"
#include "module/sha256.h"
#include "cryptominisat4/cryptominisat.h"
#include "collector/solverprinter.h"
#include "collector/modulgraph.h"

#define THREAD_NUM 4
#define MAX_TEST 2

using std::vector;
using namespace CMSat;

void signalHandler(int signum) {
    exit(1);
}

void *calculate(void* producer) {
    Sha256 sha256;

    SATSolver solver;
    solver.set_verbosity(0);
//    solver.log_to_file("solver.log");
    SolverPrinter printer(&solver);
    sha256.create(&printer);

    ModulGraph graph;
    for (unsigned i = 0; i < 24; i++) {
        Const c(32, 0);
        c.setStart(i * 32);
        c.create(&graph);
    }
    sha256.create(&graph);

    graph.calcDistances();

    vector<unsigned> v;
    while (((Producer*) producer)->getWork(v)) {
        if (graph.getDistance(v) < 5) continue;

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

    static Producer producer(MAX_TEST);
    for (unsigned i = 0; i < 49328; i++) producer.addVar(i);

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
