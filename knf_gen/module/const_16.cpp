#include "const_16.h"

using std::vector;
using namespace CMSat;

Const_16::Const_16(uint16_t value) : Modul(16, 0, 1) {
    this->value = value;
    output = 0;
}

Const_16::~Const_16() {
}

void Const_16::setValue(uint16_t value) {
    this->value = value;
}

void Const_16::create(Printer* printer) {
    for (unsigned i = 16; i > 0; i--) {
        vector<Lit> clause;
        clause.push_back(Lit(output + i - 1, !(value >> (i - 1) & 1)));
        printer->create(false, clause);
    }
}
