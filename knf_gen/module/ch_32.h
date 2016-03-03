#ifndef __CH_32_H__
#define __CH_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Ch_32 : public Modul {
    public:
        Ch_32();
        ~Ch_32();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__CH_32_H__
