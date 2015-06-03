#ifndef __BSIG1_H__
#define __BSIG1_H__

#include "modul.h"
#include "bsig0.h"

class Bsig1 : public Modul {
    public:
        Bsig1();
        ~Bsig1();

        unsigned getAdditionalVarCount();
        unsigned getClauseCount();

        void create(Printer* printer);
    protected:

    private:
        Bsig0 bsig0;
};

#endif //__BSIG1_H__
