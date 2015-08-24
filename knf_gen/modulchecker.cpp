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

using std::cout;
using std::vector;
using std::set;
using std::map;
using std::setw;
using std::ofstream;
using namespace CMSat;

//#define USE_IRRED 1

#ifdef USE_IRRED
    #define INPUT_FILE "2015-08-11_dump/000_irred.dimacs"
    #define OUTPUT_FILE "2015-08-11_dump/000_irred_outside.dimacs"
    #define OUTPUT_PREFIX "2015-08-11_dump/000_irred_"
#else
    #define INPUT_FILE "2015-08-11_dump/000_learned.dimacs"
    #define OUTPUT_FILE "2015-08-11_dump/000_learned_outside.dimacs"
    #define OUTPUT_PREFIX "2015-08-11_dump/000_learned_"
#endif

int main() {
    ModulDB printer;
    Sha256 sha256;
    sha256.create(&printer);

    ofstream outsideFile(OUTPUT_FILE);
    map<const char*, set<vector<Lit>, compareClause> > storage;

    DimacsParser dp(INPUT_FILE);
    vector<Lit> learned;
    while (dp.getNextClause(learned)) {
        ModulEntry* mod = printer.isInSingleModul(learned);
        if (mod == NULL) {
            printClause(outsideFile, learned);
            continue;
        }

/*
        if (learned.size() < 4) {
            mod->print(std::cout);
            std::cout << "OLD: ";
            printClause(std::cout, learned);
        }
*/
        for (unsigned r = mod->ranges.size() - 1; r > 0; r--) {
            unsigned distance = mod->ranges[r].first - (mod->ranges[r-1].first + mod->ranges[r-1].second);
            for (unsigned l = 0; l < learned.size(); l++) {
                if (learned[l].var() >= mod->ranges[r].first) learned[l] = Lit(learned[l].var() - distance, learned[l].sign());
            }
        }
        unsigned distance = mod->ranges[0].first;
        for (unsigned l = 0; l < learned.size(); l++) {
            learned[l] = Lit(learned[l].var() - distance, learned[l].sign());
        }
/*
        if (learned.size() < 4) {
            std::cout << "NEW: ";
            printClause(std::cout, learned);
        }
*/
        storage[mod->name].insert(learned);
    }

    for (map<const char*, set<vector<Lit>, compareClause> >::iterator it = storage.begin(); it != storage.end(); ++it) {
        char filename[100];
        snprintf(filename, 100, "%s%s.dimacs", OUTPUT_PREFIX, it->first);
        ofstream file(filename);
        for (set<vector<Lit>, compareClause>::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {
            printClause(file, *it1);
        }
        file.close();
    }
    outsideFile.close();

    return 0;
}
