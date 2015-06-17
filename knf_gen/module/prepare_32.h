#ifndef __PREPARE_32_H__
#define __PREPARE_32_H__

#include "modul.h"
#include "../minunit.h"

class Prepare_32 : public Modul {
    public:
        Prepare_32();
        ~Prepare_32();

        void create(Printer* printer);
        MU_TEST_H(test);
};

#endif //__PREPARE_32_H__
