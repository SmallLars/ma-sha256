#ifndef __DES_ROUND_H__
#define __DES_ROUND_H__

#include <stdint.h>

#include "modul.h"
#include "../common/minunit.h"

class Des_Round : public Modul {
    public:
        Des_Round(uint32_t round);
        ~Des_Round();

        unsigned* getStats();
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        uint32_t round;
        static unsigned stats[STATS_LENGTH];
};

#endif //__DES_ROUND_H__
