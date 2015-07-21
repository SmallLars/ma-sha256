#ifndef __BSIG1_32_H__
#define __BSIG1_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Bsig1_32 : public Modul {
    public:
        Bsig1_32();
        ~Bsig1_32();

        unsigned* getStats();
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__BSIG1_32_H__
