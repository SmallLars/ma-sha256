#ifndef __ADDER_PREPARE_32_H__
#define __ADDER_PREPARE_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Adder_Prepare_32 : public Modul {
    public:
        Adder_Prepare_32();
        ~Adder_Prepare_32();

        void create(Printer* printer);
        MU_TEST_H(test);
};

#endif //__ADDER_PREPARE_32_H__
