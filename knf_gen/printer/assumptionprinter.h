#ifndef __ASSUMPTIONPRINTER_H__
#define __ASSUMPTIONPRINTER_H__

#include "printer.h"

class AssumptionPrinter : public Printer {
    public:
        AssumptionPrinter(std::vector<CMSat::Lit>* lits);
        ~AssumptionPrinter();

        void create(bool xOR, const std::vector<CMSat::Lit>& vars);
    protected:

    private:
        std::vector<CMSat::Lit>* lits;
};

#endif //__ASSUMPTIONPRINTER_H__
