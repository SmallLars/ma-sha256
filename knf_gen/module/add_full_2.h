#ifndef __ADD_FULL_2_H__
#define __ADD_FULL_2_H__

#include "modul.h"
#include "../common/minunit.h"

class Add_Full_2 : public Modul {
    public:
        Add_Full_2();
        ~Add_Full_2();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__ADD_FULL_2_H__
