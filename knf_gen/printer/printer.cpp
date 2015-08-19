#include "printer.h"

#include <cmath>

using std::vector;
using namespace CMSat;

Printer::Printer() {
}

Printer::~Printer() {
}

void Printer::newModul(unsigned level, const char* name, Modul* modul) {}

void Printer::create(bool xOR, const std::vector<CMSat::Lit>& vars) {}

vector< vector<Lit> > Printer::convertXOR(const vector<Lit>& vars) {
    vector< vector<Lit> > clauses;
    if (vars.size() < 2) return clauses;

    for (unsigned i = 0; i < pow(2, vars.size()); i++) {
        unsigned result = 0;
        for (unsigned v = 1; v < vars.size(); v++) result ^= ((i >> v) & 1);
        if (result != ((i & 1) ^ vars[0].sign())) {
            vector<Lit> clause;
            for (unsigned v = 0; v < vars.size(); v++) {
                clause.push_back(Lit(vars[v].var(), (i >> v) & 1));
            }
            clauses.push_back(clause);
        }
    }
    return clauses;
}
