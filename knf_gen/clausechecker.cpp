#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <string.h>

#include "cryptominisat4/cryptominisat.h"

#include "common/dimacsparser.h"
#include "common/clausetools.h"

#include "module/sha256.h"
#include "module/shacore_32.h"
#include "module/prepare_32.h"
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

#include "collector/solverprinter.h"

using std::cout;
using std::string;
using std::vector;
using std::setw;
using std::ifstream;
using std::ofstream;
using namespace CMSat;

void excel_out_init(std::ostream& out, const char* name, int rowcount);
void excel_out_final(std::ostream& out);

int main(int argc, const char* argv[]) {
    if (argc < 3 || argc > 4) {
        cout << "Usage: clausechecker <MODUL> <FILE> [INFONUM]" << "\n";
        cout << "MODUL =\n";
        cout << "  1 = SHA256\n";
        cout << "  2 = ShaCore_32\n";
        cout << "  3 = Prepare_32\n";
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

    bool singleSolver = true;

    Modul* modul;
    switch (atoi(argv[1])) {
        case  1: modul = new Sha256; singleSolver = false; break;
        case  2: modul = new ShaCore_32; break;
        case  3: modul = new Prepare_32; break;
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

    SATSolver solver_1;
    solver_1.set_verbosity(0);
    SolverPrinter printer_1(&solver_1);
    modul->create(&printer_1);

    SATSolver solver_2;
    solver_1.set_verbosity(0);
    if (!singleSolver) {
        SolverPrinter printer_2(&solver_2);
        modul->create(&printer_2);
    }

    delete modul;

    char fileprefix[256];
    strncpy(fileprefix, argv[2], 256);
    *strrchr(fileprefix, '.') = 0;

    char filename[256];
    snprintf(filename, 256, "%s.dimacs", fileprefix);

    unsigned linecount = 0;
    DimacsParser dp(filename);
    vector<Lit> learned;
    while (dp.getNextClause(learned)) linecount++;
    dp.reset();

    unsigned rowcount = 30;
    if (argc == 4) {
        rowcount = atoi(argv[3]);
    }

    snprintf(filename, 256, "%s.txt", fileprefix);
    ofstream bin_out(filename);
    bin_out << "{";
    snprintf(filename, 256, "%s.xls", fileprefix);
    ofstream excel_out(filename);
    excel_out_init(excel_out, fileprefix, rowcount);

    cout << "Start checking " << linecount << " clauses with " << (singleSolver ? 1 : 2) << " SAT-Instances:\n";

    unsigned counter = 0;
    for (unsigned c = 1; dp.getNextClause(learned); ++c) {
        if ((c - 1) % rowcount == 0) {
            bin_out << "},\n{";
            excel_out << "      </tr>\n";
            excel_out << "      <tr>\n";
            excel_out << "        <td class=\"empty\">&nbsp;</td><td class=\"empty\">&nbsp;</td><td class=\"empty\">&nbsp;</td>\n";
            excel_out << "        <td class=\"headY\"><b>" << (c - 1) / rowcount << "</b></td>\n";
        } else {
            bin_out << ",";
        }
//        if (c <= 840) continue;

        for (unsigned i = 0; i < learned.size(); i++) learned[i] ^= 1;

        lbool ret;
        if (singleSolver || c <= linecount / 2) {
          ret = solver_1.solve(&learned);
        } else {
          ret = solver_2.solve(&learned);
        }
        cout << "\r" << c << " / " << linecount << " (" << ((c - 1) / rowcount) << " / " << ((c - 1) % rowcount) << ")" << std::flush;
        if (ret == l_False) {
            ++counter;
            bin_out << "1";
            excel_out << "  	    <td class=\"value1\">1</td>\n";
            cout << " PASS: ";
            printClause(cout, learned, true);
        } else {
            bin_out << "0";
            excel_out << "  	    <td class=\"value0\">0</td>\n";
//            cout << " FAIL: ";
//            printClause(cout, learned, true);
        }
    }

    cout << "\nFound " << counter << " valid conflictclauses.\n";

    excel_out_final(excel_out);
    excel_out.close();
    bin_out << "}";
    bin_out.close();

    return 0;
}

void excel_out_init(std::ostream& out, const char* name, int rowcount) {
    out << "<html xmlns:v=\"urn:schemas-microsoft-com:vml\"\n";
    out << "xmlns:o=\"urn:schemas-microsoft-com:office:office\"\n";
    out << "xmlns:x=\"urn:schemas-microsoft-com:office:excel\"\n";
    out << "xmlns=\"http://www.w3.org/TR/REC-html40\">\n";
    out << "  <head>\n";
    out << "    <xml>\n";
    out << "      <o:OfficeDocumentSettings><o:Colors><o:Color>\n";
    out << "        <o:Index>14</o:Index>\n";
    out << "        <o:RGB>#99CC66</o:RGB>\n";
    out << "      </o:Color></o:Colors></o:OfficeDocumentSettings>\n";
    out << "      <x:ExcelWorkbook><x:ExcelWorksheets><x:ExcelWorksheet>\n";
    out << "	    <x:Name>" << strrchr(name, '/') + 1 << "</x:Name>\n";
    out << "        <x:WorksheetOptions><x:Selected/></x:WorksheetOptions>\n";
    out << "      </x:ExcelWorksheet></x:ExcelWorksheets></x:ExcelWorkbook>\n";
    out << "    </xml>\n";
    out << "    <title></title>\n";
    out << "	  <style type=\"text/css\">\n";
    out << "      .head {\n";
    out << "        background-color: #99CC66;\n";
    out << "      }\n";
    out << "      .headX {\n";
    out << "        border-left: .5pt solid black;\n";
    out << "        border-bottom: .5pt solid black;\n";
    out << "        background-color: #99CC66;\n";
    out << "      }\n";
    out << "      .headY {\n";
    out << "        border-top: .5pt solid black;\n";
    out << "        border-right: .5pt solid black;\n";
    out << "        background-color: #99CC66;\n";
    out << "      }\n";
    out << "      .value0 {\n";
    out << "        border-top: .5pt hairline black;\n";
    out << "        border-left: .5pt hairline black;\n";
    out << "        text-align: center;\n";
    out << "        background-color: #999999;\n";
    out << "      }\n";
    out << "      .value1 {\n";
    out << "        border-top: .5pt hairline black;\n";
    out << "        border-left: .5pt hairline black;\n";
    out << "        text-align: center;\n";
    out << "        background-color: #FFFFFF;\n";
    out << "      }\n";
    out << "	  </style>\n";
    out << "  </head>\n";
    out << "  <body>\n";
    out << "    <table>\n"; // cellspacing=\"0\" border=\"0\">\n";
    out << "      <col width=24 span=256 style=\"mso-width-source: userset; mso-width-alt: 877; width: 18pt;\">\n";
    out << "      <tr><td></td></tr><tr><td></td></tr><tr><td></td></tr><tr><td></td></tr>\n";
    out << "      <tr>\n";
    out << "        <td></td><td></td><td></td>\n";
    out << "        <td class=\"head\"></td>\n";
    for (int i = 0; i < rowcount; i++) {
        out << "        <td class=\"headX\"><b>" << i << "</b></td>\n";
    }
}

void excel_out_final(std::ostream& out) {
    out << "      </tr>\n";
    out << "    </table>\n";
    out << "  </body>\n";
    out << "</html>\n";
}
