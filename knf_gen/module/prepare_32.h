#ifndef __PREPARE_32_H__
#define __PREPARE_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Prepare_32 : public Modul {
    public:
        Prepare_32();
        ~Prepare_32();

        unsigned* getStats();
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__PREPARE_32_H__
