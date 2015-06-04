#include "const.h"

using std::vector;
using namespace CMSat;

Const::Const(uint32_t value) : Modul(0) {
    this->value = value;
    output = 0;
}

Const::~Const() {
}

unsigned Const::getAdditionalVarCount() {
    return 32;
}

unsigned Const::getClauseCount() {
    return 32;
}

void Const::setValue(uint32_t value) {
    this->value = value;
}

void Const::create(Printer* printer) {
    for (unsigned i = 32; i > 0; i--) {
        vector<Lit> clause;
        clause.push_back(Lit(output + i - 1, !(value >> (i - 1) & 1)));
        printer->create(false, clause);
    }
}
