#include <vector>
#include <set>
#include <stdio.h>
#include <iomanip>
#include <signal.h>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>

#include "cryptominisat4/cryptominisat.h"

#include "common/dimacsparser.h"

using std::cout;
using std::set;
using std::setw;
using std::vector;
using std::set;
using std::ofstream;

using namespace CMSat;

struct clausecomp {
    bool operator() (const vector<Lit>& lhs, const vector<Lit>& rhs) const {
        if (lhs.size() != rhs.size())
            return lhs.size() < rhs.size();

        for (unsigned i = 0; i < lhs.size(); i++) {
           if (lhs[i].toInt() != rhs[i].toInt())
               return lhs[i].toInt() < rhs[i].toInt();
        }

        return false;
    }
};

int main() {
    set<vector<Lit>, clausecomp> original;
    set<vector<Lit>, clausecomp> irreducible;
    set<vector<Lit>, clausecomp> reducible;

    const char* filename[4] = {"00_sha256.dimacs", "01_sha256.dimacs", "10_sha256.dimacs", "11_sha256.dimacs"};
    for (unsigned i = 0; i < 4; i++) {
        DimacsParser parser(filename[i]);
        vector<Lit> clause;
        while (parser.getNextClause(clause)) {
            original.insert(clause);
        }
    }

    cout << "Einlesen der Eingabe beendet.\n";

    unsigned counter = 0;
    for (unsigned i = 1; i <= 23; i++) {
        char filename[33];
        sprintf(filename, "2015-08-11_dump/%03u_irred.dimacs", i);
        DimacsParser parser(filename);
        vector<Lit> clause;
        while (parser.getNextClause(clause)) {
            if (original.find(clause) != original.end()) {
                counter++;
                continue;
            }
            irreducible.insert(clause);
        }
        cout << "\rEinlesen von " << filename << " beendet." << std::flush;
    }
    cout << "\nirreducible size: " << irreducible.size() << " killed: " << counter  << "\n";

    counter = 0;
    for (unsigned i = 1; i <= 23; i++) {
        char filename[35];
        sprintf(filename, "2015-08-11_dump/%03u_learned.dimacs", i);
        DimacsParser parser(filename);
        vector<Lit> clause;
        while (parser.getNextClause(clause)) {
            if (original.find(clause) != original.end()) {
                counter++;
                continue;
            }/*
            if (irreducible.find(clause) != irreducible.end()) {
                counter++;
                continue;
            }*/
            reducible.insert(clause);
        }
        cout << "\rEinlesen von " << filename << " beendet." << std::flush;
    }
    cout << "\nreducible (learned) size: " << reducible.size() << " killed: " << counter << "\n";

    set< vector<Lit> >::iterator it;

    ofstream i_out("2015-08-11_dump/000_irred.dimacs");
    for (it = irreducible.begin(); it != irreducible.end(); ++it) {
        for (unsigned i = 0; i < it->size(); i++) {
            if ((*it)[i].sign() == 1) i_out << "-";
            i_out << (*it)[i].var() + 1 << " ";
        }
        i_out << "0\n";
    }
    i_out.close();
    irreducible.clear();

    ofstream r_out("2015-08-11_dump/000_learned.dimacs");
    for (it = reducible.begin(); it != reducible.end();/* ++it*/) {
        for (unsigned i = 0; i < it->size(); i++) {
            if ((*it)[i].sign() == 1) r_out << "-";
            r_out << (*it)[i].var() + 1 << " ";
        }
        r_out << "0\n";
        reducible.erase(it++);
        cout << "\rZu schreiben: " << std::setw(7) << reducible.size() << std::flush;
    }
    r_out.close();

    return 0;
}
