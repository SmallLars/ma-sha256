#include <string>
#include <vector>
#include <iomanip>
#include <fstream>

#include "cryptominisat4/cryptominisat.h"

#include "common/dimacsparser.h"
#include "common/clausetools.h"

#include "module/sha256.h"
#include "module/shacore_32.h"
#include "module/add_prepare_32.h"
#include "module/add_ssig_32.h"
#include "module/add_b0maj_32.h"
#include "module/add_b1ch_32.h"
#include "module/ssig0_32.h"
#include "module/ssig1_32.h"
#include "module/bsig0_32.h"
#include "module/bsig1_32.h"
#include "module/maj_32.h"
#include "module/bsig1_32.h"
#include "module/ch_32.h"
#include "module/add_32.h"
#include "module/add_half_1.h"
#include "module/add_full_1.h"
#include "module/add_last_1.h"
#include "module/add_half_2.h"
#include "module/add_full_2.h"
#include "module/add_last_2.h"
#include "module/add_half_3.h"
#include "module/add_full_3.h"
#include "module/add_last_3.h"

#include "printer/solverprinter.h"

using std::cout;
using std::string;
using std::vector;
using std::setw;
using std::ifstream;
using namespace CMSat;

int main(int argc, const char* argv[]) {
    if (argc < 3 || argc > 4) {
        cout << "Usage: clausechecker <MODUL> <FILE> [INFONUM]" << "\n";
        cout << "MODUL =\n";
        cout << "  1 = SHA256\n";
        cout << "  2 = ShaCore_32\n";
        cout << "  3 = Add_Prepare_32\n";
        cout << "  4 = Add_SSig_32\n";
        cout << "  5 = Add_B0Maj_32\n";
        cout << "  6 = Add_B1Ch_32\n";
        cout << "  7 = Ssig0_32\n";
        cout << "  8 = Ssig1_32\n";
        cout << "  9 = Bsig0_32\n";
        cout << " 10 = Bsig1_32\n";
        cout << " 11 = Maj_32\n";
        cout << " 12 = Bsig1_32\n";
        cout << " 13 = Ch_32\n";
        cout << " 14 = Add_32\n";
        cout << " 15 = Add_Half_1\n";
        cout << " 16 = Add_Full_1\n";
        cout << " 17 = Add_Last_1\n";
        cout << " 18 = Add_Half_2\n";
        cout << " 19 = Add_Full_2\n";
        cout << " 20 = Add_Last_2\n";
        cout << " 21 = Add_Half_3\n";
        cout << " 22 = Add_Full_3\n";
        cout << " 23 = Add_Last_3\n";
        cout << "FILE = Dimacs files with conflictclauses to check.\n";
        cout << "INFONUM = Rowcount and linecount for table.\n";
        return 0;
    }

    SolverConf config;
    config.verbosity = 0;
    SATSolver solver(config);
//    solver.set_num_threads(4);

    SolverPrinter printer(&solver);

    Modul* modul;
    switch (atoi(argv[1])) {
        case  1: modul = new Sha256; break;
        case  2: modul = new ShaCore_32; break;
        case  3: modul = new Add_Prepare_32; break;
        case  4: modul = new Add_Ssig_32; break;
        case  5: modul = new Add_B0Maj_32; break;
        case  6: modul = new Add_B1Ch_32; break;
        case  7: modul = new Ssig0_32; break;
        case  8: modul = new Ssig1_32; break;
        case  9: modul = new Bsig0_32; break;
        case 10: modul = new Bsig1_32; break;
        case 11: modul = new Maj_32; break;
        case 12: modul = new Bsig1_32; break;
        case 13: modul = new Ch_32; break;
        case 14: modul = new Add_32; break;
        case 15: modul = new Add_Half_1; break;
        case 16: modul = new Add_Full_1; break;
        case 17: modul = new Add_Last_1; break;
        case 18: modul = new Add_Half_2; break;
        case 19: modul = new Add_Full_2; break;
        case 20: modul = new Add_Last_2; break;
        case 21: modul = new Add_Half_3; break;
        case 22: modul = new Add_Full_3; break;
        case 23: modul = new Add_Last_3; break;
        default: cout << "Unknown Modul\n"; return 0;
    }
    modul->create(&printer);
    delete modul;

    unsigned linecount = 0;
    DimacsParser dp(argv[2]);
    vector<Lit> learned;
    while (dp.getNextClause(learned)) linecount++;
    dp.reset();

    unsigned rowcount = 30;
    if (argc == 4) {
        rowcount = atoi(argv[3]);
    }

    cout << "Start checking " << linecount << " clauses:\n";

    unsigned counter = 0;
    for (unsigned c = 1; dp.getNextClause(learned); ++c) {
//        if (c <= 580) continue;

        for (unsigned i = 0; i < learned.size(); i++) learned[i] ^= 1;

        lbool ret = solver.solve(&learned);
        cout << "\r" << c << " / " << linecount << " (" << ((c - 1) / rowcount) << " / " << ((c - 1) % rowcount) << ")" << std::flush;
        if (ret == l_False) {
            ++counter;
            cout << " PASS: ";
            printClause(cout, learned, true);
        } else {
//            cout << " FAIL: ";
//            printClause(cout, learned, true);
        }
    }
    cout << "\nFound " << counter << " valid conflictclauses.\n";

    return 0;
}
