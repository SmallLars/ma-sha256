#ifndef __ADDER_32_H__
#define __ADDER_32_H__

#include "modul.h"
#include "../minunit.h"

class Adder_32 : public Modul {
    public:
        Adder_32();
        ~Adder_32();

        void create(Printer* printer);
        MU_TEST(test);
};

#endif //__ADDER_32_H__
