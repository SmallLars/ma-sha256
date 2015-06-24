#ifndef __CONST_H__
#define __CONST_H__

#include "modul.h"

class Const : public Modul {
    public:
        Const(unsigned bitWidth, uint32_t value);
        ~Const();

        void setValue(uint32_t value);
        void create(Printer* printer);
    private:
        uint32_t value;
};

#endif //__CONST_H__
