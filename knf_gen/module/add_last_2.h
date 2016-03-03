#ifndef __ADD_LAST_2_H__
#define __ADD_LAST_2_H__

#include "modul.h"
#include "../common/minunit.h"

class Add_Last_2 : public Modul {
    public:
        Add_Last_2();
        ~Add_Last_2();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__ADD_LAST_2_H__
