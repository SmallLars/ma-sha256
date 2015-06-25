#ifndef __SHACORE_EX2_32_H__
#define __SHACORE_EX2_32_H__

#include "modul.h"
#include "../minunit.h"

class ShaCore_Ex2_32 : public Modul {
    public:
        ShaCore_Ex2_32(uint32_t value);
        ~ShaCore_Ex2_32();

        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        uint32_t value;
};

#endif //__SHACORE_EX2_32_H__
