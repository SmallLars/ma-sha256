#ifndef __DES_SBOX_4_H__
#define __DES_SBOX_4_H__

#include "modul.h"
#include "../common/minunit.h"

class Des_SBox_4 : public Modul {
    public:
        Des_SBox_4();
        ~Des_SBox_4();

        unsigned* getStats();
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__DES_SBOX_4_H__
