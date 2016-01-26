#ifndef __SHA256_H__
#define __SHA256_H__

#include "modul.h"
#include "clausecreator.h"
#include "../common/minunit.h"

class Sha256 : public Modul {
    public:
        Sha256();
        ~Sha256();

        unsigned* getStats();
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];

        void clause_5_39(ClauseCreator &cc);
        void clause_5_46(ClauseCreator &cc);
        void clause_5_46_M(ClauseCreator &cc);
        void clause_4_39(ClauseCreator &cc);
        void clause_4_39_M(ClauseCreator &cc);
        void clause_4_46(ClauseCreator &cc);
        void clause_4_46_M(ClauseCreator &cc);
        void clause_4_48(ClauseCreator &cc);
        void clause_3_39(ClauseCreator &cc);
        void clause_3_43(ClauseCreator &cc);
        void clause_3_48(ClauseCreator &cc);
        void clause_2_48(ClauseCreator &cc);
        void clause_1_48(ClauseCreator &cc);
};

#endif //__SHA256_H__
