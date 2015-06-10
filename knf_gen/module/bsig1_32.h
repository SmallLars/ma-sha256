#ifndef __BSIG1_32_H__
#define __BSIG1_32_H__

#include "modul.h"

class Bsig1_32 : public Modul {
    public:
        Bsig1_32();
        ~Bsig1_32();

        void create(Printer* printer);
};

#endif //__BSIG1_32_H__
