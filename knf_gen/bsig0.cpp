#include "bsig0.h"

void Bsig0::create(void (Modul::*createX) (bool, const std::vector<signed>&)) {
    for (unsigned i = 0; i < 32; i++) {
        std::vector<signed> clause;
        clause.push_back(32 + i);
        clause.push_back((i +  2) % 32);
        clause.push_back((i + 13) % 32);
        clause.push_back((i + 22) % 32);
        (this->*createX)(true, clause);
    }
}
