#ifndef __SUB_32_H__
#define __SUB_32_H__

#include "modul.h"
#include "../common/minunit.h"

class Sub_32 : public Modul {
    public:
        Sub_32();
        ~Sub_32();

        void create(Printer* printer);
        MU_TEST_H(test);
};

#endif //__SUB_32_H__
