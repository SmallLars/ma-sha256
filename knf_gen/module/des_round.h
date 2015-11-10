#ifndef __DES_ROUND_H__
#define __DES_ROUND_H__

#include "modul.h"
#include "../common/minunit.h"

#include "des_f.h"

class Des_F : public Modul {
    public:
        Des_Round(uint round);
        ~Des_Round();

        unsigned* getStats();
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        uint round;
        static unsigned stats[STATS_LENGTH];
};

#endif //__DES_ROUND_H__
