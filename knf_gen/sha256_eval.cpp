#include <vector>
#include <stdio.h>
#include <stdlib.h>
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

int main(int argc, const char* argv[]) {
    int fixedBits;
    if (argc < 2 || (fixedBits = atoi(argv[1])) < 1) {
        cout << "sha256_eval <fixedBits>\nfixedBits needs to be > 0\n";
        return 0;
    }

    signal(SIGINT, signalHandler);

    long numCPU = sysconf( _SC_NPROCESSORS_ONLN );
    cout << "CPUNUM: " << numCPU << "\n";

    uint32_t input[16];
    sha256_padding(input, "Das ist eine Eingabe aus der ein Hash erstellt wird.");
    // 27931f0e 7e53670d dbec1a1c e23e21b4 663c63c0 d17117ee 1a934bc0 c294dbe9
    uint32_t state[8] = {0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19};
    uint32_t output[8] = {0x27931f0e, 0x7e53670d, 0xdbec1a1c, 0xe23e21b4, 0x663c63c0, 0xd17117ee, 0x1a934bc0, 0xc294dbe9};

    SATSolver solver;
    solver.set_num_threads(numCPU);
    solver.set_verbosity(0);

    time_t rawtime;
    time(&rawtime);
    char filename[64];
    unsigned len = strftime(filename, 64, "%Y-%m-%d %H:%M:%S", localtime(&rawtime));
    sprintf(filename + len, " - sha256_8 - %02u", fixedBits);
    solver.add_sql_tag("filename", filename);

    solverToInterrupt = &solver;

    // Eingabe
    for (unsigned i = 13; i < 16; i++) solver_writeInt(solver, i * 32, 32, input[i]);
    // Status
    for (unsigned i = 16; i < 24; i++) solver_writeInt(solver, i * 32, 32, state[i - 16]);
    // SHA-256
    SolverPrinter printer(&solver);
    Sha256 sha256;
    sha256.create(&printer);
    // Ausgabe setzen
    solver_writeInt(solver, sha256.getOutput(), fixedBits, output[0]);

    time_t start_time = time(0);

    lbool ret = solver.solve();
    if (ret == l_False) {
        cout << "Nicht lösbar.\n";
        return 0;
    }

    cout << "  Lösung für " << fixedBits << " Bits gefunden zum Zeitpunkt ";
    printTime(cout, time(0) - start_time);
    cout << "\n  Ausgabe:";
    for (unsigned i = 0; i < 8; i++) {
        printf(" %08lx", solver_readInt(solver, sha256.getOutput() + i * 32, 32));
    }
    cout << "\n  Eingabe:";
    for (unsigned i = 0; i < 16; i++) {
        if (i == 8) cout << "\n          ";
        printf(" %08lx", solver_readInt(solver, i * 32, 32));
    }
    cout << "\n";

    return 0;
}
