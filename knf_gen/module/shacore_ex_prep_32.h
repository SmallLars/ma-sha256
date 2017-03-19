#ifndef __SHACORE_EX_PREP_32_H__
#define __SHACORE_EX_PREP_32_H__

#include "modul.h"
#include "../common/minunit.h"

class ShaCore_Ex_Prep_32 : public Modul {
    public:
        ShaCore_Ex_Prep_32(uint32_t value);
        ~ShaCore_Ex_Prep_32();

        void setValue(uint32_t value);
        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        uint32_t value;

        static unsigned stats[STATS_LENGTH];
};

#endif //__SHACORE_EX_PREP_32_H__
