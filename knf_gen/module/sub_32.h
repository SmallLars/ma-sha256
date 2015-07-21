#ifndef __SUB_32_H__
#define __SUB_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Sub_32 : public Modul {
    public:
        Sub_32();
        ~Sub_32();

        unsigned* getStats();
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__SUB_32_H__
