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
        getStream() << "x-";
        for (unsigned i = 0; i < vars.size(); i++) getStream() << vars[i].var() << " ";
        getStream() << "0\n";
    } else {
        for (unsigned i = 0; i < vars.size(); i++) {
            if (vars[i].sign()) getStream() << "-";
            getStream() << vars[i].var() << " ";
        }
        getStream() << "0\n";
    }
}

void DimacsFilePrinter::printHeader() {
    getStream() << "p cnf " << varCount << " " << clauseCount << "\n";
}
