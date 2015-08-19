#ifndef __CONST_H__
#define __CONST_H__

#include "modul.h"
#include "../common/minunit.h"

class Const : public Modul {
    public:
        Const(unsigned bitWidth, uint32_t value);
        ~Const();

        void setValue(uint32_t value);
        unsigned* getStats();
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        uint32_t value;

        static unsigned stats[STATS_LENGTH];
};

#endif //__CONST_H__
