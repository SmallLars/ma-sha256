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
    //TODO
    getStream() << varCount << "\n";
/*
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
*/
}

void TTFilePrinter::printHeader() {
    getStream() << ".i " << varCount << "\n.o 1\n.type r\n";
}
