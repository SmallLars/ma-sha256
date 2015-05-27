#include "const.h"

Const::Const(uint32_t value) {
    this->value = value;
}

Const::~Const() {
}

void Const::create(void (Modul::*createX) (bool, const std::vector<CMSat::Lit>&)) {
    for (unsigned i = 32; i > 0; i--) {
        std::vector<CMSat::Lit> clause;
        clause.push_back(CMSat::Lit(i - 1, !(value >> (i - 1) & 1)));
        (this->*createX)(false, clause);
    }
}
