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

int main() {
    ModulDB printer;
    Sha256 sha256;
    sha256.create(&printer);

    DimacsParser dp("2015-08-11_dump/000_irred.dimacs");
    ofstream inside_out("2015-08-11_dump/000_irred_inside.dimacs");
    ofstream outside_out("2015-08-11_dump/000_irred_outside.dimacs");
//    DimacsParser dp("2015-08-11_dump/000_learned.dimacs");
//    ofstream inside_out("2015-08-11_dump/000_learned_inside.dimacs");
//    ofstream outside_out("2015-08-11_dump/000_learned_outside.dimacs");
    vector<Lit> learned;
    unsigned counter = 0;
    while (dp.getNextClause(learned)) {
        bool ret = printer.isInSingleModul(learned);
        if (ret) {
            printClause(inside_out, learned);
        } else {
            counter++;
            printClause(outside_out, learned);
        }
    }
    inside_out.close();
    outside_out.close();
    cout << "Modulübergreifend: " << counter << "\n";

    return 0;
}
