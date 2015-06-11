#ifndef __SSIG1_32_H__
#define __SSIG1_32_H__

#include "modul.h"
#include "../minunit.h"

class Ssig1_32 : public Modul {
    public:
        Ssig1_32();
        ~Ssig1_32();

        void create(Printer* printer);
        MU_TEST_H(test);
};

#endif //__SSIG1_32_H__
