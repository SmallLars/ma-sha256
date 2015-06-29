#ifndef __ADDER_4_H__
#define __ADDER_4_H__

#include "modul.h"
#include "../common/minunit.h"

class Adder_4 : public Modul {
    public:
        Adder_4();
        ~Adder_4();

        void create(Printer* printer);
        MU_TEST_H(test);
};

#endif //__ADDER_4_H__
