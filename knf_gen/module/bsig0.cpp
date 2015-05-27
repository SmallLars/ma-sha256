#include "bsig0.h"

using std::vector;
using namespace CMSat;

void Bsig0::create(void (Modul::*createX) (bool, const vector<Lit>&)) {
    for (unsigned i = 0; i < 32; i++) {
        std::vector<Lit> clause;
        clause.push_back(Lit(32 + i, true));
        clause.push_back(Lit((i +  2) % 32, true));
        clause.push_back(Lit((i + 13) % 32, true));
        clause.push_back(Lit((i + 22) % 32, true));
        (this->*createX)(true, clause);
    }
}
