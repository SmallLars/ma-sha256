#include <vector>
#include <set>
#include <stdio.h>
#include <iomanip>
#include <signal.h>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <algorithm>

#include "cryptominisat4/cryptominisat.h"

#include "common/dimacsparser.h"
#include "common/clausetools.h"

using std::cout;
using std::set;
using std::setw;
using std::vector;
using std::set;
using std::ofstream;
using std::find;
using std::flush;

using namespace CMSat;

int main(int argc, const char* argv[]) {
    if (argc != 4) {
        cout << "Usage: clausesorter <IN_FILE> <OUT_FILE> <DUP_FILE>\n";
        return 0;
    }

    unsigned linecount = 0;
    DimacsParser dp(argv[1]);
    vector<Lit> clause;
    while (dp.getNextClause(clause)) linecount++;
    dp.reset();

    unsigned counter = 0;
    set<vector<Lit>, compareClause> clause_pool;
    while (dp.getNextClause(clause)) {
        counter++;
        if (clause_pool.find(clause) != clause_pool.end()) {
            // cout << "REDUNDANT: ";
            // printClause(cout, clause);
        } else {
            clause_pool.insert(clause);
        }
        printf("\r%u / %u - Already removed %zu redundant clauses.", counter, linecount, counter - clause_pool.size());
        cout << flush;
    }
    cout << "\nFinished reading. Try to remove useless clauses.\n";
    counter -= clause_pool.size();

    ofstream dup_out(argv[3]);
    unsigned counter1 = 0;
    set< vector<Lit> >::reverse_iterator rit;
    set< vector<Lit> >::iterator it;
    for (rit = clause_pool.rbegin(); rit != clause_pool.rend(); ++rit) {
        counter1++;
        for (it = clause_pool.begin(); it->size() < rit->size(); ++it) {
            bool check = true;
            for (unsigned i = 0; i < it->size(); ++i) {
                if (find(rit->begin(), rit->end(), it->at(i)) == rit->end()) {
                    check = false;
                    break;
                }
            }
            if (check) {
                printClause(dup_out, *rit);
                clause_pool.erase(*(rit--));
                it = clause_pool.end();
            }
        }
        printf("\r%u / %u - Already removed %zu useless clauses.", counter1, linecount - counter, linecount - counter - clause_pool.size());
        printf(" Working on Clause with %zu literals.", rit->size());
        cout << flush;
    }
    dup_out.close();

    ofstream i_out(argv[2]);
    for (it = clause_pool.begin(); it != clause_pool.end(); ++it) {
        printClause(i_out, *it);
    }
    i_out.close();

    cout << "Read " << linecount << " clauses, wrote " << clause_pool.size() << " clauses.\n";

    return 0;
}
