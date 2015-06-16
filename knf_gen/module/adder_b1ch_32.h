#ifndef __ADDER_B1CH_32_H__
#define __ADDER_B1CH_32_H__

#include "modul.h"
#include "../minunit.h"

class Adder_B1Ch_32 : public Modul {
    public:
        Adder_B1Ch_32();
        ~Adder_B1Ch_32();

        void create(Printer* printer);
        MU_TEST_H(test);
};

#endif //__ADDER_B1CH_32_H__
