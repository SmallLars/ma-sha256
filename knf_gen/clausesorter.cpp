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
#include <algorithm>

#include "cryptominisat4/cryptominisat.h"

#include "common/dimacsparser.h"
#include "common/clausetools.h"

using std::cout;
using std::set;
using std::map;
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
        // if (clause_pool.find(clause) != clause_pool.end()) {
            // cout << "REDUNDANT: ";
            // printClause(cout, clause);
        // } else {
            clause_pool.insert(clause);
        // }
        if (counter != linecount && counter % 100) continue;
        printf("\r%u / %u - Already removed %zu redundant clauses.", counter, linecount, counter - clause_pool.size());
        cout << flush;
    }
    cout << "\nFinished reading. Try to remove useless clauses.\n";
    counter -= clause_pool.size();

    map< Lit, vector< const vector<Lit>* > > lookup_table;
    set< vector<Lit> >::iterator it;
    for (it = clause_pool.begin(); it != clause_pool.end(); ++it) {
        for (unsigned i = 0; i < it->size(); ++i) {
            lookup_table[it->at(i)].push_back(&*it);
        }
    }

    set<vector<Lit>, compareClause> dup_clauses;
    unsigned counter1 = 0;
    for (it = clause_pool.begin(); it != clause_pool.end(); ++it) {
        counter1++;
        for (unsigned i = 0; i < it->size(); ++i) {
            vector< const vector<Lit>* >::iterator clause;
            clause = lookup_table[it->at(i)].begin();
            while (clause != lookup_table[it->at(i)].end()) {
                if ((*clause)->size() >= it->size()) {
                    ++clause;
                    continue;
                }

                bool check = true;
                for (unsigned l = 0; l < (*clause)->size(); ++l) {
                    if (find(it->begin(), it->end(), (*clause)->at(l)) == it->end()) {
                        check = false;
                        break;
                    }
                }
                if (!check) {
                    ++clause;
                    continue;
                }

                dup_clauses.insert(*it);
                i = it->size() - 1;
                break;
            }
        }
        if (counter1 != linecount - counter && counter1 % 100) continue;
        printf("\r%u / %u - Already removed %zu useless clauses.", counter1, linecount - counter, dup_clauses.size());
        cout << flush;
    }

    ofstream dup_out(argv[3]);
    for (it = dup_clauses.begin(); it != dup_clauses.end(); ++it) {
        clause_pool.erase(*it);
        printClause(dup_out, *it);
    }
    dup_out.close();

    ofstream i_out(argv[2]);
    for (it = clause_pool.begin(); it != clause_pool.end(); ++it) {
        printClause(i_out, *it);
    }
    i_out.close();

    cout << "\nRead " << linecount << " clauses, wrote " << clause_pool.size() << " clauses.\n";

    return 0;
}
