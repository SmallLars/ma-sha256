#ifndef __SHACORE_EX1_32_H__
#define __SHACORE_EX1_32_H__

#include "modul.h"
#include "../common/minunit.h"

class ShaCore_Ex1_32 : public Modul {
    public:
        ShaCore_Ex1_32(uint32_t value);
        ~ShaCore_Ex1_32();

        void setValue(uint32_t value);
        unsigned* getStats();
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        uint32_t value;

        static unsigned stats[STATS_LENGTH];
};

#endif //__SHACORE_EX1_32_H__
