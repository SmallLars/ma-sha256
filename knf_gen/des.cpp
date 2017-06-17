#include <vector>
#include <stdio.h>
#include <iomanip>
#include <signal.h>
#include <time.h>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>

#include "cryptominisat4/cryptominisat.h"

#include "common/destools.h"
#include "common/dimacsparser.h"
#include "common/solvertools.h"
#include "common/general.h"

#include "module/const.h"
#include "module/sha256.h"
#include "module/des_encrypt.h"

#include "collector/counter.h"
#include "collector/logger.h"
#include "collector/solverprinter.h"
#include "collector/bufferedsolverprinter.h"
#include "collector/assumptionprinter.h"

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

    solver->open_file_and_dump_red_clauses("dump/65_learned.dimacs");
    solver->open_file_and_dump_irred_clauses("dump/65_irred.dimacs");

    if (signum == SIGINT) _exit(1);
}

int main() {
    signal(SIGINT, signalHandler);
    signal(SIGUSR1, signalHandler);
    mkdir("dump", 0777);

    const char* plaintext = "LarsJens";
    const char* key = "JensLars";
    const char* ciphertext = "\x70\xD8\x08\x26\xB1\x59\xEE\x30";

    long numCPU = sysconf( _SC_NPROCESSORS_ONLN );
    cout << "CPUNUM: " << numCPU << "\n";

    SATSolver solver;
    solver.set_num_threads(numCPU);
    solver.set_verbosity(0);
    solver.set_no_bva();
//    solver.log_to_file("solver.txt");

    time_t rawtime;
    time(&rawtime);
    char filename[64];
    strftime(filename, 64, "des %Y-%m-%d %H:%M:%S", localtime(&rawtime));
    solver.add_sql_tag("filename", filename);

    solverToInterrupt = &solver;

    vector<Lit> assumptions;
    AssumptionPrinter ap(&assumptions);

    SolverPrinter printer(&solver);
    uint64_t value;

    // Plaintext
    value = initial_permutation(str_to_int(plaintext));
    Const c1(64, value, false);
    c1.setStart(0);
    c1.create(&ap);

    cout << "  1 /   3: Eingabe gesetzt.\n";

    Des_Encrypt des_encrypt;
    des_encrypt.create(&printer);

    cout << "  2 /   3: Kern definiert.\n";

    // Ciphertext
    value = final_permutation_reverse(str_to_int(ciphertext));
    Const c2(64, value, false);
    c2.setStart(des_encrypt.getOutput());
    c2.create(&ap);

    cout << "  3 /   3: Ausgabe gesetzt.\n";

    time_t start_time = time(0);
    for (unsigned r = 65; r <= assumptions.size(); r++) {
        cout << setw(2) << r - 64 << " / " << assumptions.size() - 64 << ":" << flush;

        vector<Lit> as(assumptions.begin(), assumptions.begin() + r);
        lbool ret = solver.solve(&as);
        if (ret == l_False) {
            cout << "Nicht lösbar.\n";
            return 0;
        }

        cout << " Lösung gefunden zum Zeitpunkt ";
        printTime(cout, time(0) - start_time);
        cout << "\n";
        printf("  SAT-RES: %016lx\n", solver_readInt_msb(solver, des_encrypt.getOutput(), 64));
        printf("  Ausgabe: %016lx\n", final_permutation(solver_readInt_msb(solver, des_encrypt.getOutput(), 64)));
        printf("  Key:     %016lx\n", key_initial_permutation_reverse(solver_readInt_msb(solver, 64, 56)));

        char red_name[24];
        char irred_name[22];
        sprintf(red_name, "dump/%03u_learned.dimacs", r - 64);
        sprintf(irred_name, "dump/%03u_irred.dimacs", r - 64);
        solver.open_file_and_dump_red_clauses(red_name);
        solver.open_file_and_dump_irred_clauses(irred_name);
    }

    solver.print_stats();

    return 0;
}
