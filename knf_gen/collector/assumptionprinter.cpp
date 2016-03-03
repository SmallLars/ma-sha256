#include "assumptionprinter.h"

using std::vector;
using namespace CMSat;

AssumptionPrinter::AssumptionPrinter(vector<Lit>* lits) {
    this->lits = lits;
}

AssumptionPrinter::~AssumptionPrinter() {
}

void AssumptionPrinter::create(bool xOR, const vector<Lit>& vars) {
    if (vars.size() != 1) return;

    lits->push_back(vars[0]);
}
