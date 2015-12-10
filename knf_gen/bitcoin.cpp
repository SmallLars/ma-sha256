#include <vector>
#include <stdio.h>
#include <iomanip>
#include <signal.h>
#include <time.h>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>

#include "cryptominisat4/cryptominisat.h"

#include "common/sha256tools.h"
#include "common/dimacsparser.h"
#include "common/solvertools.h"
#include "common/general.h"

#include "module/const.h"
#include "module/sha256.h"

#include "printer/counter.h"
#include "printer/logger.h"
#include "printer/solverprinter.h"
#include "printer/bufferedsolverprinter.h"
#include "printer/assumptionprinter.h"

using std::cout;
using std::vector;
using std::setw;
using std::flush;
using namespace CMSat;

SATSolver* solverToInterrupt;

void signalHandler(int signum) {
    std::cerr << "*** INTERRUPTED ***\n";

    SATSolver* solver = solverToInterrupt;
    solver->interrupt_asap();

    solver->add_in_partial_solving_stats();
    solver->print_stats();

    _exit(1);
}

int main() {
    signal(SIGINT, signalHandler);

    // https://blockexplorer.com/block/000000000000003e3d9fc1d3ffb3a502302797beb27c28a3fbb7000003efd472

    uint32_t input_1[32] = {
        // version, prev block
        0x01000000, 0x660675bc, 0xaf38efda, 0xc1d8d4f1, 0xac71425e, 0x7ee406b7, 0x55524edb, 0xe3040000,
        //        , merkle
        0x00000000, 0x6b7e2727, 0x1fc5ae0a, 0xb735c5d1, 0x56bdfaf1, 0xee344164, 0xf4a9107d, 0x6cf0411c,
        //        , time      , bits      , nonce     , padding
        0xfd6fd7ea, 2004092497, 0xb1a6051a, 0xf103fe2a, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000280
    };

    uint32_t input_2[16] = {
        0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19,
        0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000100
    };

    uint32_t state_2[8] = {0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19};

    sha256_calc(input_2, input_1);
//    sha256_calc(input_2, input_1 + 16);
//    sha256_calc(state_2, input_2);

    for (unsigned i = 0; i < 8; i++) {
        printf("%08x ", state_2[i]);
    }
    printf("\n");

    long numCPU = sysconf( _SC_NPROCESSORS_ONLN );
    cout << "CPUNUM: " << numCPU << "\n";

    SATSolver solver;
    solver.set_verbosity(0);
    solver.set_num_threads(numCPU);
//    solver.log_to_file("solver.txt");

    time_t rawtime;
    time(&rawtime);
    char filename[64];
    strftime(filename, 64, "sha256_7 %Y-%m-%d %H:%M:%S", localtime(&rawtime));
    solver.add_sql_tag("filename", filename);

    solverToInterrupt = &solver;

    SolverPrinter printer(&solver);

    // Eingabe
    for (unsigned i = 0; i < 16; i++) {
        if (i == 3) continue; // nonce
        Const c(32, input_1[i + 16]);
        c.setStart(i * 32);
        c.create(&printer);
    }
    for (unsigned i = 0; i < 16; i++) {
        Const c(32, input_2[i]);
        c.setStart(512 + i * 32);
        c.create(&printer);
    }
    for (unsigned i = 0; i < 8; i++) {
        Const c(32, state_2[i]);
        c.setStart(1024 + i * 32);
        c.create(&printer);
    }
    cout << "  1 /   3: Eingabe gesetzt.\n";

    Sha256 sha256_1;
    sha256_1.setStart(1280);
    sha256_1.create(&printer);
    cout << "  2 /   3: Kern 1/2 definiert.\n";

    vector<unsigned> subinputs;
    for (unsigned i = 0; i < 8; i++) subinputs.push_back(sha256_1.getOutput() + i * 32);
    for (unsigned i = 0; i < 16; i++) subinputs.push_back(768 + i * 32);
    Sha256 sha256_2;
    sha256_2.setInputs(subinputs);
    sha256_2.setStart(sha256_1.getOutput() + 256);
    sha256_2.create(&printer);
    cout << "  3 /   3: Kern 2/2 definiert.\n";

    vector<Lit> assumptions;
    AssumptionPrinter ap(&assumptions);
    // Ausgabe setzen
    for (unsigned i = 0; i < 8; i++) {
        Const c(32, 0);
        c.setStart(sha256_2.getOutput() + (7 - i) * 32);
        c.create(&ap);
    }

    time_t start_time = time(0);
    for (unsigned r = 1; r <= assumptions.size(); r++) {
        cout << setw(3) << r << " / " << assumptions.size() << ":" << flush;

        vector<Lit> as(assumptions.begin(), assumptions.begin() + r);
        lbool ret = solver.solve(&as);
        if (ret == l_False) {
            cout << "Nicht lösbar.\n";
            return 0;
        }

        printf(" Lösung gefunden zum Zeitpunkt ");
        printTime(cout, time(0) - start_time);
        printf(" mit Nonce: %08lx\n       Ausgabe:", solver_readInt(solver, 96, 32));
        for (unsigned i = 0; i < 8; i++) {
            printf(" %08lx", solver_readInt(solver, sha256_2.getOutput() + i * 32, 32));
        }
        printf("\n");
    }

    solver.print_stats();

    return 0;
}
