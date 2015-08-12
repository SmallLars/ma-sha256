#include "clauseprinter.h"

#include <algorithm>

using std::ostream;
using std::vector;
using namespace CMSat;

void printClause(ostream& out, const vector<Lit>& clause) {
    vector<Lit> sorted(clause);
    std::sort(sorted.begin(), sorted.end());
    for (unsigned i = 0; i < sorted.size(); i++) {
        if (sorted[i].sign()) out << "-";
        out << (sorted[i].var() + 1) << " ";
    }
    out << "0\n";
}
