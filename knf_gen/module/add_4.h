#ifndef __ADD_4_H__
#define __ADD_4_H__

#include "modul.h"
#include "../common/minunit.h"

class Add_4 : public Modul {
    public:
        Add_4();
        ~Add_4();

        unsigned* getStats();
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__ADD_4_H__
