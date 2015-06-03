#ifndef __CONST_H__
#define __CONST_H__

#include "modul.h"

class Const : public Modul {
    public:
        Const(uint32_t value);
        ~Const();

        unsigned getAdditionalVarCount();
        unsigned getClauseCount();

        void setValue(uint32_t);

        void create(Printer* printer);
    protected:

    private:
        uint32_t value;
};

#endif //__CONST_H__
