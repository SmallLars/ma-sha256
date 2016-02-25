#ifndef __SSIG1_32_H__
#define __SSIG1_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Ssig1_32 : public Modul {
    public:
        Ssig1_32();
        ~Ssig1_32();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__SSIG1_32_H__
