#ifndef __ASSUMPTIONPRINTER_H__
#define __ASSUMPTIONPRINTER_H__

#include "collector.h"

class AssumptionPrinter : public Collector {
    public:
        AssumptionPrinter(std::vector<CMSat::Lit>* lits);
        ~AssumptionPrinter();

        void create(bool xOR, const std::vector<CMSat::Lit>& vars);
    protected:

    private:
        std::vector<CMSat::Lit>* lits;
};

#endif //__ASSUMPTIONPRINTER_H__
