#ifndef __ADDER_B0MAJ_32_H__
#define __ADDER_B0MAJ_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Adder_B0Maj_32 : public Modul {
    public:
        Adder_B0Maj_32();
        ~Adder_B0Maj_32();

        void create(Printer* printer);
        MU_TEST_H(test);
};

#endif //__ADDER_B0MAJ_32_H__
