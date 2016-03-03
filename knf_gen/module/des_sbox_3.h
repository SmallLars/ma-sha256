#ifndef __DES_SBOX_3_H__
#define __DES_SBOX_3_H__

#include "modul.h"
#include "../common/minunit.h"

class Des_SBox_3 : public Modul {
    public:
        Des_SBox_3();
        ~Des_SBox_3();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__DES_SBOX_3_H__
