#ifndef __DES_F_H__
#define __DES_F_H__

#include "modul.h"
#include "../common/minunit.h"

#include "des_box_1.h"
#include "des_box_2.h"
#include "des_box_3.h"
#include "des_box_4.h"
#include "des_box_5.h"
#include "des_box_6.h"
#include "des_box_7.h"
#include "des_box_8.h"

class Des_F : public Modul {
    public:
        Des_F(uint round);
        ~Des_F();

        unsigned* getStats();
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        uint round;
        static unsigned stats[STATS_LENGTH];
};

#endif //__DES_F_H__
