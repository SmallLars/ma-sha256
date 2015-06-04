#ifndef __BSIG0_32_H__
#define __BSIG0_32_H__

#include "modul.h"

class Bsig0_32 : public Modul {
    public:
        Bsig0_32();
        ~Bsig0_32();

        unsigned getAdditionalVarCount();
        unsigned getClauseCount();

        void create(Printer* printer);
    protected:
};

#endif //__BSIG0_32_H__
