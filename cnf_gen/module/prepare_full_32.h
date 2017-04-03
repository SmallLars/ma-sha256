#ifndef __PREPARE_FULL_32_H__
#define __PREPARE_FULL_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Prepare_Full_32 : public Modul {
    public:
        Prepare_Full_32();
        ~Prepare_Full_32();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__PREPARE_FULL_32_H__
