#include "ttfileprinter.h"

using std::vector;
using namespace CMSat;

TTFilePrinter::TTFilePrinter(const char* filename, unsigned varCount) : FilePrinter(filename) {
    this->varCount = varCount;

    printHeader();
}

TTFilePrinter::~TTFilePrinter() {
}

void TTFilePrinter::create(bool xOR, const vector<Lit>& vars) {
    if (xOR) {
       // TODO
    } else { 
        vector<char> values(varCount, -1);
        for (unsigned i = 0; i < vars.size(); i++) {
            values[vars[i].var()] = (vars[i].sign() ? 1 : 0);
        }
        for (unsigned i = 0; i < varCount; i++) {
            switch (values[i]) {
                case 0: getStream() << "0"; break;
                case 1: getStream() << "1"; break;
                default: getStream() << "-";
            }
        }
        getStream() << "|0\n";
    }
}

void TTFilePrinter::printHeader() {
    getStream() << ".i " << varCount << "\n.o 1\n.type r\n";
}
