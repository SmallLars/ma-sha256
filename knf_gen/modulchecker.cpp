#include <vector>
#include <stdio.h>
#include <iomanip>
#include <signal.h>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>

#include "cryptominisat4/cryptominisat.h"

#include "common/dimacsparser.h"
#include "common/clauseprinter.h"

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
using std::setw;
using std::ofstream;
using namespace CMSat;

#define INPUT_FILE "2015-08-11_dump/000_irred.dimacs"
#define OUTPUT_PREFIX "2015-08-11_dump/000_irred_"

//#define INPUT_FILE "2015-08-11_dump/000_learned.dimacs"
//#define OUTPUT_PREFIX "2015-08-11_dump/000_learned_"

int main() {
    ModulDB printer;
    Sha256 sha256;
    sha256.create(&printer);

    DimacsParser dp(INPUT_FILE);

    vector<Lit> learned;
    while (dp.getNextClause(learned)) {
        ModulEntry* mod = printer.isInSingleModul(learned);
        if (mod != NULL) {
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
        }

        char filename[100];
        snprintf(filename, 100, "%s%s.dimacs", OUTPUT_PREFIX, (mod == NULL ? "outside" : mod->name));
        ofstream file(filename, std::ofstream::app);
        printClause(file, learned);
        file.close();
    }

    return 0;
}
