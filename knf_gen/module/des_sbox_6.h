#ifndef __DES_SBOX_6_H__
#define __DES_SBOX_6_H__

#include "modul.h"
#include "../common/minunit.h"

class Des_SBox_6 : public Modul {
    public:
        Des_SBox_6();
        ~Des_SBox_6();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__DES_SBOX_6_H__
