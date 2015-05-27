#include "maj.h"

using std::vector;
using namespace CMSat;

// MAJ(x; y; z) = (x AND y) XOR (x AND z) XOR (y AND z)
// (!m | x | y) & (!m | x | z) & (!m | y | z) & (m | !x | !y) & (m | !x | !z) & (m | !y | !z)

// x =  0 -  31
// y = 32 -  63
// z = 64 -  95
// m = 96 - 127

Maj::Maj() {
    start = 96;
    inputs.push_back(0);
    inputs.push_back(32);
    inputs.push_back(64);
}

Maj::~Maj() {
}

unsigned Maj::getInputCount() {
    return 3;
}

void Maj::create(void (Modul::*createX) (bool, const vector<Lit>&)) {
    vector<Lit> clause;
    for (unsigned i = 0; i < 32; i++) {
        // (!m | x | y)
        clause.clear();
        clause.push_back(Lit(i + start, true));
        clause.push_back(Lit(i + inputs[0], false));
        clause.push_back(Lit(i + inputs[1], false));
        (this->*createX)(false, clause);

        // (!m | x | z)
        clause.clear();
        clause.push_back(Lit(i + start, true));
        clause.push_back(Lit(i + inputs[0], false));
        clause.push_back(Lit(i + inputs[2], false));
        (this->*createX)(false, clause);

        // (!m | y | z)
        clause.clear();
        clause.push_back(Lit(i + start, true));
        clause.push_back(Lit(i + inputs[1], false));
        clause.push_back(Lit(i + inputs[2], false));
        (this->*createX)(false, clause);

        // (m | !x | !y)
        clause.clear();
        clause.push_back(Lit(i + start, false));
        clause.push_back(Lit(i + inputs[0], true));
        clause.push_back(Lit(i + inputs[1], true));
        (this->*createX)(false, clause);

        // (m | !x | !z)
        clause.clear();
        clause.push_back(Lit(i + start, false));
        clause.push_back(Lit(i + inputs[0], true));
        clause.push_back(Lit(i + inputs[2], true));
        (this->*createX)(false, clause);

        // (m | !y | !z)
        clause.clear();
        clause.push_back(Lit(i + start, false));
        clause.push_back(Lit(i + inputs[1], true));
        clause.push_back(Lit(i + inputs[2], true));
        (this->*createX)(false, clause);
    }
}
