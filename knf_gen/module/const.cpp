#include "const.h"

using std::vector;
using namespace CMSat;

Const::Const(uint32_t value) {
    this->value = value;
}

Const::~Const() {
}

void Const::create(void (Modul::*createX) (bool, const vector<Lit>&)) {
    for (unsigned i = 32; i > 0; i--) {
        vector<Lit> clause;
        clause.push_back(Lit(i - 1, !(value >> (i - 1) & 1)));
        (this->*createX)(false, clause);
    }
}
