#ifndef __SHACORE_32_H__
#define __SHACORE_32_H__

#include "modul.h"
#include "../common/minunit.h"

class ShaCore_32 : public Modul {
    public:
        ShaCore_32();
        ~ShaCore_32();

        void create(Printer* printer);
        MU_TEST_H(test);
};

#endif //__SHACORE_32_H__
