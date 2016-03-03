#ifndef __SOLVERPRINTER_H__
#define __SOLVERPRINTER_H__

#include "collector.h"

class SolverPrinter : public Collector {
    public:
        SolverPrinter(CMSat::SATSolver* solver);
        ~SolverPrinter();

        virtual void create(bool xOR, const std::vector<CMSat::Lit>& vars);
        virtual void flush();
    protected:

    private:
        CMSat::SATSolver* solver;
};

#endif //__SOLVERPRINTER_H__
