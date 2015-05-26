#include "bsig0.h"

void Bsig0::create(void (Modul::*createX) (bool, const std::vector<signed>&)) {
    std::vector<signed> test;
    (this->*createX)(true, test);
}
