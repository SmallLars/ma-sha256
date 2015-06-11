#ifndef __BSIG0_32_H__
#define __BSIG0_32_H__

#include "modul.h"
#include "../minunit.h"

class Bsig0_32 : public Modul {
    public:
        Bsig0_32();
        ~Bsig0_32();

        void create(Printer* printer);
        MU_TEST(test);
};

#endif //__BSIG0_32_H__
