#ifndef __ADD_B0MAJ_32_H__
#define __ADD_B0MAJ_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Add_B0Maj_32 : public Modul {
    public:
        Add_B0Maj_32();
        ~Add_B0Maj_32();

        unsigned* getStats();
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__ADD_B0MAJ_32_H__
