#ifndef __DES_SBOX_8_H__
#define __DES_SBOX_8_H__

#include "modul.h"
#include "../common/minunit.h"

class Des_SBox_8 : public Modul {
    public:
        Des_SBox_8();
        ~Des_SBox_8();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__DES_SBOX_8_H__
