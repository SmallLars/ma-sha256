#ifndef __PREPARE_32_H__
#define __PREPARE_32_H__

#include "modul.h"
#include "clausecreator.h"
#include "../common/minunit.h"

class Prepare_32 : public Modul {
    public:
        Prepare_32();
        ~Prepare_32();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
        void mirror_block(ClauseCreator &cc, unsigned i, unsigned j, unsigned x, unsigned y);
};

#endif //__PREPARE_32_H__
