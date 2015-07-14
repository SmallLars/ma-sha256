#ifndef __ADD_SSIG_32_H__
#define __ADD_SSIG_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Add_Ssig_32 : public Modul {
    public:
        Add_Ssig_32();
        ~Add_Ssig_32();

        void create(Printer* printer);
        MU_TEST_H(test);
};

#endif //__ADD_SSIG_32_H__
