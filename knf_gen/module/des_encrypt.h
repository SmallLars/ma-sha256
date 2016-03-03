#ifndef __DES_ENCRYPT_H__
#define __DES_ENCRYPT_H__

#include "modul.h"
#include "../common/minunit.h"

class Des_Encrypt : public Modul {
    public:
        Des_Encrypt();
        ~Des_Encrypt();

        unsigned* getStats();
        void create(Collector* collector);
        MU_TEST_H(test);
    private:
        static unsigned stats[STATS_LENGTH];
};

#endif //__DES_ENCRYPT_H__
