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
#include "common/clausetools.h"

using std::cout;
using std::set;
using std::setw;
using std::vector;
using std::set;
using std::ofstream;

using namespace CMSat;

int main(int argc, const char* argv[]) {
    if (argc != 3) {
        cout << "Usage: clausesorter <IN_FILE> <OUT_FILE>\n";
        return 0;
    }

    unsigned counter = 0;
    set<vector<Lit>, compareClause> clause_pool;

    DimacsParser parser(argv[1]);
    vector<Lit> clause;
    while (parser.getNextClause(clause)) {
        counter++;
        clause_pool.insert(clause);
    }

    set< vector<Lit> >::iterator it;
    ofstream i_out(argv[2]);
    for (it = clause_pool.begin(); it != clause_pool.end(); ++it) {
        printClause(i_out, *it);
    }
    i_out.close();

    cout << "Read " << counter << " clauses, wrote " << clause_pool.size() << " clauses.\n";
    cout << "Found " << counter - clause_pool.size() << " redundant clauses.\n";

    return 0;
}
