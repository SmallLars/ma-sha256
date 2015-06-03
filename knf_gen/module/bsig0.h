#ifndef __BSIG0_H__
#define __BSIG0_H__

#include "modul.h"

class Bsig0 : public Modul {
    public:
        Bsig0();
        ~Bsig0();

        unsigned getAdditionalVarCount();
        unsigned getClauseCount();

        void create(Printer* printer);
    protected:
};

#endif //__BSIG0_H__
