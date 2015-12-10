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
#include <stdlib.h>

#include "cryptominisat4/cryptominisat.h"

#include "common/dimacsparser.h"
#include "common/clausetools.h"

#include "module/const.h"
#include "module/sha256.h"

#include "printer/counter.h"
#include "printer/logger.h"
#include "printer/solverprinter.h"
#include "printer/bufferedsolverprinter.h"
#include "printer/assumptionprinter.h"
#include "printer/moduldb.h"
#include "printer/modulgraph.h"

using std::cout;
using std::vector;
using std::set;
using std::map;
using std::setw;
using std::ofstream;
using namespace CMSat;

static const char* const INPUT_FILE[3]    = {"", "dump/000_irred_outside.dimacs",    "dump/000_learned_outside.dimacs"};
static const char* const OUTPUT_FILE[3]   = {"", "dump/000_irred_outside_%d.dimacs", "dump/000_learned_outside_%d.dimacs"};

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        cout << "Usage: distancechecker <MODE>" << "\n";
        cout << "MODE =\n";
        cout << "  1 = irred\n";
        cout << "  2 = learned\n";
        return 0;
    }

    int mode = atoi(argv[1]);
    if (mode < 1 || mode > 2) {
        cout << "Mode needs to be 1 (irred) or 2 (leanred)\n";
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

    printer.calcDistances();

    map<signed, unsigned> counter;

    DimacsParser dp(INPUT_FILE[mode]);
    vector<Lit> learned;
    while (dp.getNextClause(learned)) {
        unsigned count = printer.getModulCount(learned);
        unsigned dist = printer.getDistance(learned);

        counter[dist - count + 1]++;

        char filename[100];
        snprintf(filename, 100, OUTPUT_FILE[mode], dist - count + 1);
        ofstream file(filename, std::ofstream::app);
        printClause(file, learned);
        file.close();

//        char filename[100];
//        snprintf(filename, 100, "2015-08-11_dump/000_learned_outside_%u_%u.dimacs", count, dist);
//        ofstream file(filename, std::ofstream::app);
//        printClause(file, learned);
//        file.close();

//        if (dist > count) {
//            cout << learned.size() << " (" << count << "): " << dist << "\n";
//        }

        cout << "\r" << learned.size();
    }
    cout << "\n";

    for (map<signed, unsigned>::iterator it = counter.begin(); it != counter.end(); ++it) {
        cout << it->first << ": " << it->second << "\n";
    }

    return 0;
}
