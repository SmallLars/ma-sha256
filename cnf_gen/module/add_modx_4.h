#ifndef __ADD_MODX_4_H__
#define __ADD_MODX_4_H__

#include "modul.h"
#include "../common/minunit.h"

class Add_ModX_4 : public Modul {
    public:
        Add_ModX_4();
        ~Add_ModX_4();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__ADD_MODX_4_H__
