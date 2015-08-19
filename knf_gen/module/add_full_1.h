#ifndef __ADD_FULL_1_H__
#define __ADD_FULL_1_H__

#include "modul.h"
#include "../common/minunit.h"

class Add_Full_1 : public Modul {
    public:
        Add_Full_1();
        ~Add_Full_1();

        unsigned* getStats();
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__ADD_FULL_1_H__
