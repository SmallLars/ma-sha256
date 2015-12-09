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

using std::cout;
using std::vector;
using std::set;
using std::map;
using std::setw;
using std::ofstream;
using namespace CMSat;

static const char* const INPUT_FILE[3]    = {"", "dump/000_irred.dimacs",         "dump/000_learned.dimacs"};
static const char* const OUTPUT_FILE[3]   = {"", "dump/000_irred_outside.dimacs", "dump/000_learned_outside.dimacs"};
static const char* const OUTPUT_PREFIX[3] = {"", "dump/000_irred_",               "dump/000_learned_"};

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        cout << "Usage: modulchecker <MODE>" << "\n";
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

    ModulDB printer;
    Sha256 sha256;
    sha256.create(&printer);

    ofstream outsideFile(OUTPUT_FILE[mode]);
    map<const char*, set<vector<Lit>, compareClause> > storage;

    unsigned linecount = 0;
    DimacsParser dp(INPUT_FILE[mode]);
    vector<Lit> learned;
    while (dp.getNextClause(learned)) linecount++;
    dp.reset();

    cout << "Start checking " << linecount << " clauses:\n";
    for (unsigned i = 1; dp.getNextClause(learned); i++) {
        cout << "\r" << i << " / " << linecount << std::flush;

        ModulEntry* mod = printer.isInSingleModul(learned);
        if (mod == NULL) {
            printClause(outsideFile, learned);
            continue;
        }
/*
        if (strcmp(mod->name, "ShaCore_32") == 0 && learned.size() < 4) {
            mod->print(std::cout);
            std::cout << "OLD: ";
            printClause(std::cout, learned);
        }
*/
        unsigned offset = 0;
        vector<Lit> normalized;
        for (unsigned r = 0; r < mod->ranges.size(); r++) {
            for (unsigned l = 0; l < learned.size(); l++) {
                if (learned[l].var() >= mod->ranges[r].first && learned[l].var() < mod->ranges[r].first + mod->ranges[r].second) {
                    normalized.push_back(Lit(learned[l].var() - mod->ranges[r].first + offset, learned[l].sign()));
                    learned.erase(learned.begin() + l--);
                }
            }
            offset += mod->ranges[r].second;
        }
/*
        if (strcmp(mod->name, "ShaCore_32") == 0 && normalized.size() < 4) {
            std::cout << "NEW: ";
            printClause(std::cout, normalized);
        }
*/
        storage[mod->name].insert(normalized);
    }
    cout << "\n";

    for (map<const char*, set<vector<Lit>, compareClause> >::iterator it = storage.begin(); it != storage.end(); ++it) {
        char filename[100];
        snprintf(filename, 100, "%s%s.dimacs", OUTPUT_PREFIX[mode], it->first);
        ofstream file(filename);
        for (set<vector<Lit>, compareClause>::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {
            printClause(file, *it1);
        }
        file.close();
    }
    outsideFile.close();

    return 0;
}
