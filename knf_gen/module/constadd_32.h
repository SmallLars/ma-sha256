#ifndef __CONSTADD_32_H__
#define __CONSTADD_32_H__

#include "modul.h"
#include "../common/minunit.h"

class ConstAdd_32 : public Modul {
    public:
        ConstAdd_32(uint32_t value);
        ~ConstAdd_32();

        void setValue(uint32_t value);
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        uint32_t value;
};

#endif //__CONSTADD_32_H__
