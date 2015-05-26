#include "bsig0.h"

void Bsig0::create(void (Modul::*createX) (bool, const std::vector<CMSat::Lit>&)) {
    for (unsigned i = 0; i < 32; i++) {
        std::vector<CMSat::Lit> clause;
        clause.push_back(CMSat::Lit(32 + i, true));
        clause.push_back(CMSat::Lit((i +  2) % 32, true));
        clause.push_back(CMSat::Lit((i + 13) % 32, true));
        clause.push_back(CMSat::Lit((i + 22) % 32, true));
        (this->*createX)(true, clause);
    }
}
