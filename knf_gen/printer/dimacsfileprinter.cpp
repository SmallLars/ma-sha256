#include "dimacsfileprinter.h"

#include <algorithm>

using std::vector;
using namespace CMSat;

DimacsFilePrinter::DimacsFilePrinter(const char* filename, unsigned varCount, unsigned clauseCount) : FilePrinter(filename) {
    this->varCount = varCount;
    this->clauseCount = clauseCount;

    printHeader();
}

DimacsFilePrinter::~DimacsFilePrinter() {
}

void DimacsFilePrinter::create(bool xOR, const vector<Lit>& vars) {
#ifndef XOR_SUPPORT
    if (xOR) {
        vector< vector<Lit> > clauses = convertXOR(vars);
        for (unsigned i = 0; i < clauses.size(); i++) {
             create(false, clauses[i]);
        }
        return;
    }
#endif

    if (xOR) {
        getStream() << (vars[0].sign() ? "x" : "x-");
        for (unsigned i = 0; i < vars.size(); i++) getStream() << (vars[i].var() + 1) << " ";
        getStream() << "0\n";
        return;
    }

    vector<Lit> sorted_vars(vars);
    std::sort(sorted_vars.begin(), sorted_vars.end());
    for (unsigned i = 0; i < sorted_vars.size(); i++) {
        if (sorted_vars[i].sign()) getStream() << "-";
        getStream() << (sorted_vars[i].var() + 1) << " ";
    }
    getStream() << "0\n";
}

void DimacsFilePrinter::printHeader() {
    getStream() << "p cnf " << varCount << " " << clauseCount << "\n";
}
