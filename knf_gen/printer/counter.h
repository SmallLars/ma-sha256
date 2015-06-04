#ifndef __COUNTER_H__
#define __COUNTER_H__

#include "printer.h"

class Counter : public Printer {
    public:
        Counter();
        ~Counter();

        unsigned getVarCount();
        unsigned getClauseCount();

        void create(bool xOR, const std::vector<CMSat::Lit>& vars);
    protected:

    private:
        unsigned varCount;
        unsigned clauseCount;
};

#endif //__COUNTER_H__
