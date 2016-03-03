#ifndef __ADD_B1CH_32_H__
#define __ADD_B1CH_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Add_B1Ch_32 : public Modul {
    public:
        Add_B1Ch_32();
        ~Add_B1Ch_32();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__ADD_B1CH_32_H__
