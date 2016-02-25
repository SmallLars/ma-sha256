#ifndef __MAJ_32_H__
#define __MAJ_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Maj_32 : public Modul {
    public:
        Maj_32();
        ~Maj_32();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__MAJ_32_H__
