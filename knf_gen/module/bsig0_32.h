#ifndef __BSIG0_32_H__
#define __BSIG0_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Bsig0_32 : public Modul {
    public:
        Bsig0_32();
        ~Bsig0_32();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__BSIG0_32_H__
