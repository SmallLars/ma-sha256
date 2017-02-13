#include <stdlib.h>

#include "cryptominisat4/cryptominisat.h"

#include "common/sha256tools.h"
#include "common/solvertools.h"

#include "module/const.h"
#include "module/miter.h"

#include "collector/solverprinter.h"

using std::cout;
using std::flush;
using namespace CMSat;

int main(int argc, const char* argv[]) {
    int equalBits;
    if (argc < 2 || (equalBits = atoi(argv[1])) < 1) {
        cout << "miter <equalBits>\nequalBits needs to be > 0\n";
        return 0;
    }

/*
    uint32_t input[16] = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                          0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x80000000, 0x00000000, 0x000001A0};
*/
    uint32_t input[16];
    sha256_padding(input, "Das ist eine Eingabe aus der ein Hash erstellt wird.");
    // 27931f0e 7e53670d dbec1a1c e23e21b4 663c63c0 d17117ee 1a934bc0 c294dbe9

    uint32_t state[8] = {0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19};

    long numCPU = sysconf( _SC_NPROCESSORS_ONLN );
    cout << "CPUNUM: " << numCPU << "\n";

    SATSolver solver;
    solver.set_num_threads(numCPU);
    solver.set_verbosity(0);
    solver.add_sql_tag("filename", "miter");

    SolverPrinter printer(&solver);

    // Eingabe
    for (unsigned i = 13; i < 16; i++) {
        Const c(32, input[i]);

        c.setStart(i * 32);
        c.create(&printer);

        c.setStart(512 + i * 32);
        c.create(&printer);
    }
    cout << "  1 / 3: Padding gesetzt.\n";

    // Status
    for (unsigned i = 0; i < 8; i++) {
        Const c(32, state[i]);
        c.setStart(1024 + i * 32);
        c.create(&printer);
    }
    cout << "  2 / 3: Status gesetzt.\n";

    Miter miter(equalBits);
    // miter.writeDimacs("miter.dimacs");
    miter.create(&printer);
    cout << "  3 / 3: Miter definiert.\n";

    lbool ret = solver.solve();
    if (ret == l_False) {
        cout << "Nicht lösbar.\n";
        return 0;
    }

    cout << "Lösung gefunden\n";
    cout << "  Eingabe 1:";
    for (unsigned i = 0; i < 16; i++) {
        if (i == 8) cout << "\n            ";
        printf(" %08lx", solver_readInt(solver, i * 32, 32));
    }
    cout << "\n  Eingabe 2:";
    for (unsigned i = 16; i < 32; i++) {
        if (i == 24) cout << "\n            ";
        printf(" %08lx", solver_readInt(solver, i * 32, 32));
    }
    cout << "\n  Ausgabe 1:";
    for (unsigned i = 0; i < 8; i++) {
        printf(" %08lx", solver_readInt(solver, miter.getOutput() + i * 32, 32));
    }
    cout << "\n  Ausgabe 2:";
    for (unsigned i = 8; i < 16; i++) {
        printf(" %08lx", solver_readInt(solver, miter.getOutput() + i * 32, 32));
    }
    cout << "\n";

    //solver.print_stats();

    return 0;
}
