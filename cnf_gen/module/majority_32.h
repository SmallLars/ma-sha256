#ifndef __MAJORITY_32_H__
#define __MAJORITY_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Majority_32 : public Modul {
    public:
        Majority_32();
        ~Majority_32();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__MAJORITY_32_H__
