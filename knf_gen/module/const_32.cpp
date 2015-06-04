#include "const_32.h"

using std::vector;
using namespace CMSat;

Const_32::Const_32(uint32_t value) : Modul(0) {
    this->value = value;
    output = 0;
}

Const_32::~Const_32() {
}

unsigned Const_32::getAdditionalVarCount() {
    return 32;
}

unsigned Const_32::getClauseCount() {
    return 32;
}

void Const_32::setValue(uint32_t value) {
    this->value = value;
}

void Const_32::create(Printer* printer) {
    for (unsigned i = 32; i > 0; i--) {
        vector<Lit> clause;
        clause.push_back(Lit(output + i - 1, !(value >> (i - 1) & 1)));
        printer->create(false, clause);
    }
}
