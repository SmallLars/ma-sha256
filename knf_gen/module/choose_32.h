#ifndef __CHOOSE_32_H__
#define __CHOOSE_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Choose_32 : public Modul {
    public:
        Choose_32();
        ~Choose_32();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__CHOOSE_32_H__
