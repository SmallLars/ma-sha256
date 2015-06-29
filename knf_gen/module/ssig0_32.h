#ifndef __SSIG0_32_H__
#define __SSIG0_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Ssig0_32 : public Modul {
    public:
        Ssig0_32();
        ~Ssig0_32();

        void create(Printer* printer);
        MU_TEST_H(test);
};

#endif //__SSIG0_32_H__
