#include "clausetools.h"

#include <algorithm>

using std::ostream;
using std::vector;
using std::set;
using std::map;
using namespace CMSat;

bool compareClause::operator() (const vector<Lit>& lhs, const vector<Lit>& rhs) const {
    if (lhs.size() != rhs.size())
        return lhs.size() < rhs.size();

    for (unsigned i = 0; i < lhs.size(); i++) {
       if (lhs[i].var() != rhs[i].var())
           return lhs[i].var() < rhs[i].var();
    }

    for (unsigned i = 0; i < lhs.size(); i++) {
       if (lhs[i].toInt() != rhs[i].toInt())
           return lhs[i].toInt() < rhs[i].toInt();
    }

    return false;
}

void printClause(ostream& out, const vector<Lit>& clause, bool revert) {
    vector<Lit> sorted(clause);
    std::sort(sorted.begin(), sorted.end());
    for (unsigned i = 0; i < sorted.size(); i++) {
        if (sorted[i].sign() != revert) out << "-";
        out << (sorted[i].var() + 1) << " ";
    }
    out << "0\n";
}

void createLookup(map< Lit, vector< const vector<Lit>* > >& lookup_table, const set<vector<Lit>, compareClause>& clause_pool) {
    for (set< vector<Lit> >::iterator it = clause_pool.begin(); it != clause_pool.end(); ++it) {
        for (unsigned i = 0; i < it->size(); ++i) {
            lookup_table[it->at(i)].push_back(&*it);
        }
    }
}

bool hasSubClause(const vector<Lit>& clause, map< Lit, vector< const vector<Lit>* > >& lookup_table) {
    for (unsigned i = 0; i < clause.size(); ++i) {
        vector< const vector<Lit>* >::iterator to_check;
        to_check = lookup_table[clause.at(i)].begin();
        while (to_check != lookup_table[clause.at(i)].end()) {
            if ((*to_check)->size() >= clause.size()) {
                ++to_check;
                continue;
            }

            bool check = true;
            for (unsigned l = 0; l < (*to_check)->size(); ++l) {
                if (find(clause.begin(), clause.end(), (*to_check)->at(l)) == clause.end()) {
                    check = false;
                    break;
                }
            }
            if (!check) {
                ++to_check;
                continue;
            }

            return true;
        }
    }

    return false;
}
