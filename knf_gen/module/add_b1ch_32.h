#ifndef __ADD_B1CH_32_H__
#define __ADD_B1CH_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Add_B1Ch_32 : public Modul {
    public:
        Add_B1Ch_32();
        ~Add_B1Ch_32();

        void create(Printer* printer);
        MU_TEST_H(test);
};

#endif //__ADD_B1CH_32_H__
