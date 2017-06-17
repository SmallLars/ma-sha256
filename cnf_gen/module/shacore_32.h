#ifndef __SHACORE_32_H__
#define __SHACORE_32_H__

#include "modul.h"
#include "../common/minunit.h"

class ShaCore_32 : public Modul {
    public:
        ShaCore_32();
        ~ShaCore_32();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__SHACORE_32_H__
