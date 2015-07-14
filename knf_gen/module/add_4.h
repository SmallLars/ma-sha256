#ifndef __ADD_4_H__
#define __ADD_4_H__

#include "modul.h"
#include "../common/minunit.h"

class Add_4 : public Modul {
    public:
        Add_4();
        ~Add_4();

        void create(Printer* printer);
        MU_TEST_H(test);
};

#endif //__ADD_4_H__
