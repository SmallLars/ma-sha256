#ifndef __ADD_PREPARE_32_H__
#define __ADD_PREPARE_32_H__

#include "modul.h"
#include "clausecreator.h"
#include "../common/minunit.h"

class Add_Prepare_32 : public Modul {
    public:
        Add_Prepare_32();
        ~Add_Prepare_32();

        unsigned* getStats();
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
        void mirror_block(ClauseCreator &cc, unsigned i, unsigned j, unsigned x, unsigned y);
};

#endif //__ADD_PREPARE_32_H__
