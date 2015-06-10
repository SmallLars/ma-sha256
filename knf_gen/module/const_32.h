#ifndef __CONST_32_H__
#define __CONST_32_H__

#include "modul.h"

class Const_32 : public Modul {
    public:
        Const_32(uint32_t value);
        ~Const_32();

        void setValue(uint32_t);
        void create(Printer* printer);
    private:
        uint32_t value;
};

#endif //__CONST_32_H__
