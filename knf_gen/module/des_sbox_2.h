#ifndef __DES_SBOX_2_H__
#define __DES_SBOX_2_H__

#include "modul.h"
#include "../common/minunit.h"

class Des_SBox_2 : public Modul {
    public:
        Des_SBox_2();
        ~Des_SBox_2();

        unsigned* getStats();
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__DES_SBOX_2_H__
