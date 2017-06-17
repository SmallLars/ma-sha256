#ifndef __MITER_H__
#define __MITER_H__

#include "modul.h"

#include "../common/minunit.h"

class Miter : public Modul {
    public:
        Miter(unsigned bits);
        ~Miter();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];

        unsigned equal_bits;
};

#endif //__MITER_H__
