#include "bsig1.h"

using std::vector;
using namespace CMSat;

Bsig1::Bsig1() : Modul(1) {
    start = 32;
    inputs.push_back(0);
}

Bsig1::~Bsig1() {
}

unsigned Bsig1::getAdditionalVarCount() {
    return 2 * bsig0.getAdditionalVarCount();
}

unsigned Bsig1::getClauseCount() {
    return 2 * bsig0.getClauseCount();
}

void Bsig1::create(Printer* printer) {
    vector<unsigned> inputs(1,0);

    inputs[0] = 0;
    bsig0.setInputs(inputs);
    bsig0.setStart(32);
    bsig0.create(printer);

    inputs[0] = 32;
    bsig0.setInputs(inputs);
    bsig0.setStart(64);
    bsig0.create(printer);
}
