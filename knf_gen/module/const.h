#ifndef __CONST_H__
#define __CONST_H__

#include "modul.h"
#include "../common/minunit.h"

class Const : public Modul {
    public:
        Const(unsigned bitWidth, uint64_t value, bool lsb = true);
        ~Const();

        void setValue(uint32_t value);
        unsigned* getStats();
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        uint64_t value;
        bool lsb;

        static unsigned stats[STATS_LENGTH];
};

#endif //__CONST_H__
