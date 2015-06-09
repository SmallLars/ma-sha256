#include "dimacsfileprinter.h"

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
    if (xOR) {
#ifdef XOR_SUPPORT
        getStream() << "x-";
        for (unsigned i = 0; i < vars.size(); i++) getStream() << (vars[i].var() + 1) << " ";
        getStream() << "0\n";
#else
        vector< vector<Lit> > clauses = convertXOR(vars);
        for (unsigned i = 0; i < clauses.size(); i++) {
             create(false, clauses[i]);
        }
#endif
        return;
    }

    for (unsigned i = 0; i < vars.size(); i++) {
        if (vars[i].sign()) getStream() << "-";
        getStream() << (vars[i].var() + 1) << " ";
    }
    getStream() << "0\n";
}

void DimacsFilePrinter::printHeader() {
    getStream() << "p cnf " << varCount << " " << clauseCount << "\n";
}
