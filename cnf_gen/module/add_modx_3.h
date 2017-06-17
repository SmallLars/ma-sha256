#ifndef __ADD_MODX_3_H__
#define __ADD_MODX_3_H__

#include "modul.h"
#include "../common/minunit.h"

class Add_ModX_3 : public Modul {
    public:
        Add_ModX_3();
        ~Add_ModX_3();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__ADD_MODX_3_H__
