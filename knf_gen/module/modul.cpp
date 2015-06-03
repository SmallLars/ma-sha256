#include "modul.h"

#include <stdio.h>
#include <assert.h>

#include "../printer/solverprinter.h"
#include "../printer/dimacsfileprinter.h"
#include "../printer/ttfileprinter.h"

using std::vector;
using namespace CMSat;

Modul::Modul(unsigned inputCount) {
    this->inputCount = inputCount;
}

Modul::~Modul() {
}

unsigned Modul::getVarCount() {
    return 32 * inputCount + getAdditionalVarCount();
}

void Modul::setStart(unsigned start) {
    this->start = start;
}

void Modul::setInputs(const vector<unsigned>& inputs) {
    assert(inputs.size() == inputCount);
    this->inputs = inputs;
}

unsigned Modul::append(SATSolver* solver) {
    SolverPrinter printer(solver);
    create(&printer);
    return 0;
}

unsigned Modul::writeDimacs(const char* filename) {
    DimacsFilePrinter printer(filename, getVarCount(), getClauseCount());
    create(&printer);
    return 0;
}

unsigned Modul::writeTT(const char* filename) {
    TTFilePrinter printer(filename, getVarCount());
    create(&printer);
    return 0;
}
