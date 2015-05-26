#ifndef __BSIG0_H__
#define __BSIG0_H__

#include "modul.h"

class Bsig0 : public Modul {
    protected:
        void create(void (Modul::*createX) (bool, const std::vector<signed>&));
};

#endif //__BSIG0_H__
