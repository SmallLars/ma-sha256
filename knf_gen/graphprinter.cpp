#include <vector>
#include <set>
#include <map>
#include <stdio.h>
#include <iomanip>
#include <signal.h>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <string.h>

#include "cryptominisat4/cryptominisat.h"

#include "common/dimacsparser.h"
#include "common/clausetools.h"

#include "module/const.h"
#include "module/sha256.h"

#include "collector/counter.h"
#include "collector/logger.h"
#include "collector/solverprinter.h"
#include "collector/bufferedsolverprinter.h"
#include "collector/assumptionprinter.h"
#include "collector/moduldb.h"
#include "collector/modulgraph.h"

using std::cout;
using std::vector;
using std::set;
using std::map;
using std::setw;
using std::ofstream;
using namespace CMSat;

int main(int argc, const char* argv[]) {
    if (argc > 2) {
        cout << "Usage: graphprinter [FILE]" << "\n";
        cout << "FILE = Dimacs file with clause to highlight.\n";
        return 0;
    }

    ModulGraph printer;

    for (unsigned i = 0; i < 24; i++) {
        Const c(32, 0);
        c.setStart(i * 32);
        c.create(&printer);
    }
    Sha256 sha256;
    sha256.create(&printer);

    if (argc == 1) {
        printer.printGraph("sha256.graph");
    } else {
        DimacsParser dp(argv[1]);
        vector<Lit> learned;
        dp.getNextClause(learned);
        printer.printGraph("sha256.graph", learned);
    }

    return 0;
}
