#ifndef __SHA256_H__
#define __SHA256_H__

#include "modul.h"
#include "../common/minunit.h"

class Sha256 : public Modul {
    public:
        Sha256();
        ~Sha256();

        unsigned* getStats();
        void create(Printer* printer);
        void getOutputs(std::vector<unsigned>& outputs);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__SHA256_H__
