#ifndef __ADDER_SSIG_32_H__
#define __ADDER_SSIG_32_H__

#include "modul.h"
#include "../minunit.h"

class Adder_Ssig_32 : public Modul {
    public:
        Adder_Ssig_32();
        ~Adder_Ssig_32();

        void create(Printer* printer);
        MU_TEST_H(test);
};

#endif //__ADDER_SSIG_32_H__
