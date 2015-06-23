#ifndef __CONST_16_H__
#define __CONST_16_H__

#include "modul.h"

class Const_16 : public Modul {
    public:
        Const_16(uint16_t value);
        ~Const_16();

        void setValue(uint16_t value);
        void create(Printer* printer);
    private:
        uint16_t value;
};

#endif //__CONST_16_H__
