#include "clausetools.h"

#include <algorithm>

using std::ostream;
using std::vector;
using namespace CMSat;

bool compareClause::operator() (const vector<Lit>& lhs, const vector<Lit>& rhs) const {
    if (lhs.size() != rhs.size())
        return lhs.size() < rhs.size();

    for (unsigned i = 0; i < lhs.size(); i++) {
       if (lhs[i].toInt() != rhs[i].toInt())
           return lhs[i].toInt() < rhs[i].toInt();
    }

    return false;
}

void printClause(ostream& out, const vector<Lit>& clause) {
    vector<Lit> sorted(clause);
    std::sort(sorted.begin(), sorted.end());
    for (unsigned i = 0; i < sorted.size(); i++) {
        if (sorted[i].sign()) out << "-";
        out << (sorted[i].var() + 1) << " ";
    }
    out << "0\n";
}
