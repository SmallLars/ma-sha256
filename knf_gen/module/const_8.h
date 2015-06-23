#ifndef __CONST_8_H__
#define __CONST_8_H__

#include "modul.h"

class Const_8 : public Modul {
    public:
        Const_8(uint8_t value);
        ~Const_8();

        void setValue(uint8_t value);
        void create(Printer* printer);
    private:
        uint8_t value;
};

#endif //__CONST_8_H__
