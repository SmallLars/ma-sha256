#ifndef __DES_SBOX_7_H__
#define __DES_SBOX_7_H__

#include "modul.h"
#include "../common/minunit.h"

class Des_SBox_7 : public Modul {
    public:
        Des_SBox_7();
        ~Des_SBox_7();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__DES_SBOX_7_H__
