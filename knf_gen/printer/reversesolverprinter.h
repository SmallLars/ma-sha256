#ifndef __REVERSESOLVERPRINTER_H__
#define __REVERSESOLVERPRINTER_H__

#include "solverprinter.h"

class ReverseSolverPrinter : public SolverPrinter {
    public:
        ReverseSolverPrinter(CMSat::SATSolver* solver);
        ~ReverseSolverPrinter();

        void create(bool xOR, const std::vector<CMSat::Lit>& vars);
        void flush();
    private:
        std::vector<bool> clauseType;
        std::vector< std::vector<CMSat::Lit> > clause;
};

#endif //__REVERSESOLVERPRINTER_H__
