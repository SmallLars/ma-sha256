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

int main() {
    ModulGraph printer;

    for (unsigned i = 0; i < 24; i++) {
        Const c(32, 0);
        c.setStart(i * 32);
        c.create(&printer);
    }

    Sha256 sha256;
    sha256.create(&printer);
    {
    DimacsParser dp("2015-08-11_dump/000_learned_outside_4.dimacs");
    vector<Lit> learned;
    dp.getNextClause(learned);
    printer.printGraph("sha256.graph", learned);
    }
/*
    printer.calcDistances();

    map<signed, unsigned> counter;

    DimacsParser dp("2015-08-11_dump/000_learned_outside.dimacs");
    vector<Lit> learned;
    while (dp.getNextClause(learned)) {
        unsigned count = printer.getModulCount(learned);
        unsigned dist = printer.getDistance(learned);

        counter[dist - count + 1]++;

        char filename[100];
        snprintf(filename, 100, "2015-08-11_dump/000_learned_outside_%d.dimacs", dist - count + 1);
        ofstream file(filename, std::ofstream::app);
        printClause(file, learned);
        file.close();

/*
        char filename[100];
        snprintf(filename, 100, "2015-08-11_dump/000_learned_outside_%u_%u.dimacs", count, dist);
        ofstream file(filename, std::ofstream::app);
        printClause(file, learned);
        file.close();
*/
/*
        if (dist > count) {
            cout << learned.size() << " (" << count << "): " << dist << "\n";
        }
* /
        cout << "\r" << learned.size();
    }
    cout << "\n";

    for (map<signed, unsigned>::iterator it = counter.begin(); it != counter.end(); ++it) {
        cout << it->first << ": " << it->second << "\n";
    }
*/
    return 0;
}
