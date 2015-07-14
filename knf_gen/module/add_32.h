#ifndef __ADD_32_H__
#define __ADD_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Add_32 : public Modul {
    public:
        Add_32();
        ~Add_32();

        void create(Printer* printer);
        MU_TEST_H(test);
};

#endif //__ADD_32_H__
