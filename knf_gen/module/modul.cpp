#include "modul.h"

#include <stdio.h>
#include <assert.h>

#include "../printer/solverprinter.h"
#include "../printer/dimacsfileprinter.h"
#include "../printer/ttfileprinter.h"
#include "../printer/counter.h"

using std::vector;
using namespace CMSat;

Modul::Modul(unsigned inputCount, unsigned inputBitWidth) {
    this->inputCount = inputCount;
    this->inputBitWidth = inputBitWidth;
}

Modul::~Modul() {
}

unsigned Modul::getVarCount() {
    Counter counter;
    create(&counter);
    return counter.getVarCount();
}

unsigned Modul::getAdditionalVarCount() {
    Counter counter;
    create(&counter);
    return counter.getVarCount() - (inputCount * inputBitWidth);
}

unsigned Modul::getClauseCount() {
    Counter counter;
    create(&counter);
    return counter.getClauseCount();
}

void Modul::setInputs(const vector<unsigned>& inputs) {
    assert(inputs.size() == inputCount);
    this->inputs = inputs;
}

void Modul::setStart(unsigned start) {
    this->start = start;
}

void Modul::setOutput(unsigned output) {
    this->output = output;
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
