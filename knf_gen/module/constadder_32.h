#ifndef __CONSTADDER_32_H__
#define __CONSTADDER_32_H__

#include "modul.h"
#include "../common/minunit.h"

class ConstAdder_32 : public Modul {
    public:
        ConstAdder_32(uint32_t value);
        ~ConstAdder_32();

        void setValue(uint32_t value);
        void create(Printer* printer);
        MU_TEST_H(test);
    private:
        uint32_t value;
};

#endif //__CONSTADDER_32_H__
