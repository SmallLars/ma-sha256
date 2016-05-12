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

#include "collector/counter.h"
#include "collector/logger.h"
#include "collector/solverprinter.h"
#include "collector/bufferedsolverprinter.h"
#include "collector/assumptionprinter.h"
#include "collector/modulgraph.h"

using std::cout;
using std::vector;
using std::setw;
using std::flush;
using namespace CMSat;

ModulGraph* graph;
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

uint32_t calc(vector<Lit>& clause, uint32_t glue) {
    int count   = graph->getModulCount(clause);
    int maxdist = graph->getDistance(clause);
    int dist    = maxdist - count + 1;

    if (dist > 0) cout << glue << ": " << dist << "(" << clause.size() << ")" << "\n";

    if (dist < 0) return 100;
    if (glue > 10) return 10;
    return glue;
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
        0x27931f0e,
        0x7e53670d,
        0xdbec1a1c,
        0xe23e21b4,
        0x663c63c0,
        0xd17117ee,
        0x1a934bc0,
        0xc294dbe9
    };

    long numCPU = sysconf( _SC_NPROCESSORS_ONLN );
    cout << "CPUNUM: " << numCPU << "\n";

    SATSolver solver;
    solver.set_num_threads(numCPU);
    solver.set_verbosity(0);
    solver.set_no_bva();
    solver.append_glue_calc(calc);
//    solver.log_to_file("solver.txt");

    time_t rawtime;
    time(&rawtime);
    char filename[64];
    strftime(filename, 64, "sha256_8 %Y-%m-%d %H:%M:%S", localtime(&rawtime));
    solver.add_sql_tag("filename", filename);

    solverToInterrupt = &solver;

    vector<Lit> assumptions;
    AssumptionPrinter ap(&assumptions);

    SolverPrinter printer(&solver);
    ModulGraph sha256graph;
    graph = &sha256graph;
    for (unsigned i = 0; i < 24; i++) {
        Const c(32, 0);
        c.setStart(i * 32);
        c.create(graph);
    }

    // Eingabe
    for (unsigned i = 13; i < 16; i++) {
        Const c(32, input[i]);
        c.setStart(i * 32);
        c.create(&ap);
    }
    cout << "  1 /   6: Eingabe gesetzt.\n";

    // Status
    for (unsigned i = 16; i < 24; i++) {
        Const c(32, state[i - 16]);
        c.setStart(i * 32);
        c.create(&ap);
    }
    cout << "  2 /   6: Status gesetzt.\n";

    Sha256 sha256;
    sha256.create(&printer);
    cout << "  3 /   6: Kern definiert.\n";
    sha256.create(graph);
    cout << "  4 /   6: Graph erzeugt.\n";
    graph->calcDistances();
    cout << "  5 /   6: Distanzen berechnet.\n";

    // Ausgabe setzen
    for (unsigned i = 0; i < 8; i++) {
        Const c(32, output[i]);
        c.setStart(sha256.getOutput() + i * 32);
        c.create(&ap);
    }
    cout << "  6 /   6: Ausgabe gesetzt.\n";

    time_t start_time = time(0);
    for (unsigned r = 353; r <= assumptions.size(); r++) {
        cout << setw(3) << r - 352 << " / " << assumptions.size() - 352 << ":" << flush;

        vector<Lit> as(assumptions.begin(), assumptions.begin() + r);
        time_t round_time = time(0);
        lbool ret = solver.solve(&as);
        if (ret == l_False) {
            cout << "Nicht lösbar.\n";
            return 0;
        }

        cout << " Lösung gefunden zum Zeitpunkt ";
        printTime(cout, time(0) - start_time);
        cout << " (";
        printTime(cout, time(0) - round_time);
        cout << ")\n  Ausgabe:";
        for (unsigned i = 0; i < 8; i++) {
            printf(" %08lx", solver_readInt(solver, sha256.getOutput() + i * 32, 32));
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
