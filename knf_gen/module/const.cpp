#include "const.h"

using std::vector;
using namespace CMSat;

Const::Const(unsigned bitWidth, uint32_t value) : Modul(bitWidth, 0, 1) {
    this->value = value;
    output = 0;
}

Const::~Const() {
}

void Const::setValue(uint32_t value) {
    this->value = value;
}

void Const::create(Printer* printer) {
    for (unsigned i = getBitWidth(); i > 0; i--) {
        vector<Lit> clause;
        clause.push_back(Lit(output + i - 1, !(value >> (i - 1) & 1)));
        printer->create(false, clause);
    }
}
