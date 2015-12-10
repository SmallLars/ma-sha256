#ifndef __DES_F_H__
#define __DES_F_H__

#include <stdint.h>

#include "modul.h"
#include "../common/minunit.h"

#include "des_sbox_1.h"
#include "des_sbox_2.h"
#include "des_sbox_3.h"
#include "des_sbox_4.h"
#include "des_sbox_5.h"
#include "des_sbox_6.h"
#include "des_sbox_7.h"
#include "des_sbox_8.h"

class Des_F : public Modul {
    public:
        Des_F(uint32_t round);
        ~Des_F();

        unsigned* getStats();
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        uint32_t round;
        static unsigned stats[STATS_LENGTH];
};

#endif //__DES_F_H__
