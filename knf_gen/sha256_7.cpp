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
    if (signum == SIGINT) std::cerr << "*** INTERRUPTED ***\n";
    else std::cerr << "*** WRITE OUT ***\n";

    SATSolver* solver = solverToInterrupt;
    if (signum == SIGINT) solver->interrupt_asap();

    solver->add_in_partial_solving_stats();
    solver->print_stats();

    solver->open_file_and_dump_red_clauses("dump/257_learned.dimacs");
    solver->open_file_and_dump_irred_clauses("dump/257_irred.dimacs");

    if (signum == SIGINT) _exit(1);
}

int main() {
    signal(SIGINT, signalHandler);
    signal(SIGUSR1, signalHandler);
    mkdir("dump", 0777);

/*
    uint32_t input[16] = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                          0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x80000000, 0x00000000, 0x000001A0};
*/
    uint32_t input[16];
    sha256_padding(input, "Das ist eine Eingabe aus der ein Hash erstellt wird.");
    // 27931f0e 7e53670d dbec1a1c e23e21b4 663c63c0 d17117ee 1a934bc0 c294dbe9

    uint32_t state[8] = {0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19};

    uint32_t output[8] = {
        0x27931f0e - state[0],
        0x7e53670d - state[1],
        0xdbec1a1c - state[2],
        0xe23e21b4 - state[3],
        0x663c63c0 - state[4],
        0xd17117ee - state[5],
        0x1a934bc0 - state[6],
        0xc294dbe9 - state[7]
    };

    SolverConf config;
    config.verbosity = 0; // 3
    config.printFullStats = 1;
    config.doSQL = true;
    config.do_bva = false;

    long numCPU = sysconf( _SC_NPROCESSORS_ONLN );
    cout << "CPUNUM: " << numCPU << "\n";

    SATSolver solver(config);
//    solver.log_to_file("solver.txt");
    solver.set_num_threads(numCPU);

    time_t rawtime;
    time(&rawtime);
    char filename[64];
    strftime(filename, 64, "sha256_7 %Y-%m-%d %H:%M:%S", localtime(&rawtime));
    solver.add_sql_tag("filename", filename);

    solverToInterrupt = &solver;

    vector<Lit> assumptions;
    AssumptionPrinter ap(&assumptions);

    SolverPrinter printer(&solver);

    // Eingabe
    for (unsigned i = 13; i < 16; i++) {
        Const c(32, input[i]);
        c.setStart(i * 32);
        c.create(&ap);
    }
    cout << "  1 /   4: Eingabe gesetzt.\n";

    // Status
    for (unsigned i = 16; i < 24; i++) {
        Const c(32, state[i - 16]);
        c.setStart(i * 32);
        c.create(&ap);
    }
    cout << "  2 /   4: Status gesetzt.\n";

    Sha256 sha256;
    sha256.create(&printer);
    cout << "  3 /   4: Kern definiert.\n";

    // Ausgabe setzen
    vector<unsigned> out_lsb;
    sha256.getOutputs(out_lsb);
    for (unsigned i = 0; i < 8; i++) {
        Const c(32, output[i]);
        c.setStart(out_lsb[i]);
        c.create(&ap);
    }
    cout << "  4 /   4: Ausgabe gesetzt.\n";

    time_t start_time = time(0);
    for (unsigned r = 353; r <= assumptions.size(); r++) {
        cout << setw(3) << r - 352 << " / " << assumptions.size() - 352 << ":" << flush;

        vector<Lit> as(assumptions.begin(), assumptions.begin() + r);
        lbool ret = solver.solve(&as);
        if (ret == l_False) {
            cout << "Nicht lösbar.\n";
            return 0;
        }

        cout << " Lösung gefunden zum Zeitpunkt ";
        printTime(cout, time(0) - start_time);
        cout << "\n  Ausgabe:";
        for (unsigned i = 0; i < 8; i++) {
            printf(" %08lx", state[i] + solver_readInt(solver, out_lsb[i], 32));
        }
        cout << "\n  Eingabe:";
        for (unsigned i = 0; i < 16; i++) {
            if (i == 8) cout << "\n          ";
            printf(" %08lx", solver_readInt(solver, i * 32, 32));
        }
        cout << "\n";

        char red_name[24];
        char irred_name[22];
        sprintf(red_name, "dump/%03u_learned.dimacs", r - 352);
        sprintf(irred_name, "dump/%03u_irred.dimacs", r - 352);
        solver.open_file_and_dump_red_clauses(red_name);
        solver.open_file_and_dump_irred_clauses(irred_name);
    }

    solver.print_stats();

    return 0;
}
