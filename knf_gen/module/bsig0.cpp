#include "bsig0.h"

using std::vector;
using namespace CMSat;

Bsig0::Bsig0() : Modul(1) {
    start = 32;
    inputs.push_back(0);
}

Bsig0::~Bsig0() {
}

unsigned Bsig0::getAdditionalVarCount() {
    return 32;
}

unsigned Bsig0::getClauseCount() {
    return 1;
}

void Bsig0::create(void (Modul::*createX) (bool, const vector<Lit>&)) {
    for (unsigned i = 0; i < 32; i++) {
        vector<Lit> clause;
        clause.push_back(Lit(start + i, true));
        clause.push_back(Lit(inputs[0] + ((i +  2) % 32), true));
        clause.push_back(Lit(inputs[0] + ((i + 13) % 32), true));
        clause.push_back(Lit(inputs[0] + ((i + 22) % 32), true));
        (this->*createX)(true, clause);
    }
}
