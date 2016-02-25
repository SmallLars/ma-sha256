#ifndef __SHACORE_EX2_32_H__
#define __SHACORE_EX2_32_H__

#include "modul.h"
#include "../common/minunit.h"

class ShaCore_Ex2_32 : public Modul {
    public:
        ShaCore_Ex2_32(uint32_t value);
        ~ShaCore_Ex2_32();

        void setValue(uint32_t value);
        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        uint32_t value;

        static unsigned stats[STATS_LENGTH];
};

#endif //__SHACORE_EX2_32_H__
