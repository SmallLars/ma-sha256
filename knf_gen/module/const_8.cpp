#include "const_8.h"

using std::vector;
using namespace CMSat;

Const_8::Const_8(uint8_t value) : Modul(8, 0, 1) {
    this->value = value;
    output = 0;
}

Const_8::~Const_8() {
}

void Const_8::setValue(uint8_t value) {
    this->value = value;
}

void Const_8::create(Printer* printer) {
    for (unsigned i = 8; i > 0; i--) {
        vector<Lit> clause;
        clause.push_back(Lit(output + i - 1, !(value >> (i - 1) & 1)));
        printer->create(false, clause);
    }
}
