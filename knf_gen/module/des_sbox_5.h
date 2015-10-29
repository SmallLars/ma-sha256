#ifndef __DES_SBOX_5_H__
#define __DES_SBOX_5_H__

#include "modul.h"
#include "../common/minunit.h"

class Des_SBox_5 : public Modul {
    public:
        Des_SBox_5();
        ~Des_SBox_5();

        unsigned* getStats();
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__DES_SBOX_5_H__
