#ifndef __SSIG0_32_H__
#define __SSIG0_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Ssig0_32 : public Modul {
    public:
        Ssig0_32();
        ~Ssig0_32();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__SSIG0_32_H__
