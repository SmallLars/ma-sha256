#ifndef __ADD_FULL_4_H__
#define __ADD_FULL_4_H__

#include "modul.h"
#include "../common/minunit.h"

class Add_Full_4 : public Modul {
    public:
        Add_Full_4();
        ~Add_Full_4();

        unsigned* getStats();
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__ADD_FULL_4_H__
