#ifndef __COUNTER_H__
#define __COUNTER_H__

#include "collector.h"

#include <set>

class Counter : public Collector {
    public:
        Counter();
        ~Counter();

        unsigned getMaxVar();
        unsigned getVarCount();
        unsigned getClauseCount();

        void create(bool xOR, const std::vector<CMSat::Lit>& vars);
    protected:

    private:
        std::set<unsigned> varSet;
        unsigned clauseCount;
        
};

#endif //__COUNTER_H__
