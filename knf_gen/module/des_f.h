#ifndef __DES_F_H__
#define __DES_F_H__

#include "modul.h"
#include "../common/minunit.h"

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
