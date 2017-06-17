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
#include <stdlib.h>

#include "cryptominisat4/cryptominisat.h"

#include "common/dimacsparser.h"
#include "common/clausetools.h"

using std::cout;
using std::set;
using std::setw;
using std::flush;
using std::vector;
using std::set;
using std::map;
using std::ofstream;

using namespace CMSat;

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        cout << "Usage: clausecollector <FILECOUNT>" << "\n";
        return 0;
    }

    int filecount = atoi(argv[1]);
    if (filecount < 1) {
        cout << "Filecount needs to be > 0\n";
        return 0;
    }

    set<vector<Lit>, compareClause> original;
    set<vector<Lit>, compareClause> irreducible;
    set<vector<Lit>, compareClause> reducible;

    const char* filename[2] = {"0_sha256.dimacs", "1_sha256.dimacs"};
    for (unsigned i = 0; i < 2; i++) {
        DimacsParser parser(filename[i]);
        vector<Lit> clause;
        while (parser.getNextClause(clause)) {
            original.insert(clause);
        }
    }

    cout << "Einlesen der Eingabe beendet.\n";

    unsigned counter = 0;
    for (int i = 1; i <= filecount; i++) {
        char filename[33];
        sprintf(filename, "dump/%03u_irred.dimacs", i);
        DimacsParser parser(filename);
        vector<Lit> clause;
        while (parser.getNextClause(clause)) {
            if (clause.size() > 10 || original.find(clause) != original.end()) {
                counter++;
                continue;
            }
            irreducible.insert(clause);
        }
        cout << "\rEinlesen von " << filename << " beendet." << std::flush;
    }
    cout << "\nirreducible size: " << irreducible.size() << " killed: " << counter  << "\n";

    counter = 0;
    for (int i = 1; i <= filecount; i++) {
        char filename[35];
        sprintf(filename, "dump/%03u_learned.dimacs", i);
        DimacsParser parser(filename);
        vector<Lit> clause;
        while (parser.getNextClause(clause)) {
            if (clause.size() > 10 || original.find(clause) != original.end() || irreducible.find(clause) != irreducible.end()) {
                counter++;
                continue;
            }
            reducible.insert(clause);
        }
        cout << "\rEinlesen von " << filename << " beendet." << std::flush;
    }
    cout << "\nreducible (learned) size: " << reducible.size() << " killed: " << counter << "\n";

    set< vector<Lit> >::iterator it;

//-----------------------
/*
    cout << "Subklauselprüfung:\n";
    unsigned progress = 0;

    map< Lit, vector< const vector<Lit>* > > lookup_table;
    createLookup(lookup_table, original);

    progress = 0;
    counter = 0;
    for (it = irreducible.begin(); it != irreducible.end(); ) {
        if (hasSubClause(*it, lookup_table)) {
          counter++;
          irreducible.erase(it++);
        } else {
          ++it;
        }
        progress++;
        cout << progress << " / " << irreducible.size() + counter << "\r" << flush;
    }
    cout << "irreducible size: " << irreducible.size() << " killed: " << counter  << "\n";

    progress = 0;
    counter = 0;
    for (it = reducible.begin(); it != reducible.end(); ) {
        if (hasSubClause(*it, lookup_table)) {
          counter++;
          reducible.erase(it++);
        } else {
          ++it;
        }
        progress++;
        cout << progress << " / " << reducible.size() + counter << "\r" << flush;
    }
    cout << "reducible size: " << reducible.size() << " killed: " << counter  << "\n";
*/
//-----------------------

    ofstream i_out("dump/000_irred.dimacs");
    for (it = irreducible.begin(); it != irreducible.end(); ++it) {
        printClause(i_out, *it);
    }
    i_out.close();
    irreducible.clear();

    ofstream r_out("dump/000_learned.dimacs");
    for (it = reducible.begin(); it != reducible.end();/* ++it*/) {
        printClause(r_out, *it);
        reducible.erase(it++);
        cout << "\rZu schreiben: " << std::setw(7) << reducible.size() << std::flush;
    }
    r_out.close();

    cout << "\n";

    return 0;
}
