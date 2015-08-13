#include <vector>
#include <stdio.h>
#include <iomanip>
#include <signal.h>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>

#include "cryptominisat4/cryptominisat.h"

#include "common/dimacsparser.h"

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
using namespace CMSat;

int main() {
    SolverConf config;
    config.verbosity = 0; // 3
    config.printFullStats = 1;
    config.doSQL = true;
    config.do_bva = false;

    SATSolver solver(config);
 //   solver.set_num_threads(8);

    SolverPrinter printer(&solver);

    Sha256 sha256;
    sha256.create(&printer);

    DimacsParser dp("test.dimacs");
    vector<Lit> learned;
    unsigned counter = 0;
    while (dp.getNextClause(learned)) {
        for (unsigned i = 0; i < learned.size(); i++) learned[i] ^= 1;

        lbool ret = solver.solve(&learned);
        if (ret == l_False) {
            cout << ++counter << ": " << learned.size() << "\n";// "\r" << std::flush;
        } else {
            cout << ++counter << " FAIL: " << learned.size() << "\n";
        }
    }

    return 0;
}
