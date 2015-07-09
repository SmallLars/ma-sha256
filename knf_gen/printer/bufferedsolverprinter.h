#ifndef __BUFFEREDSOLVERPRINTER_H__
#define __BUFFEREDSOLVERPRINTER_H__

#include "solverprinter.h"

class BufferedSolverPrinter : public SolverPrinter {
    public:
        BufferedSolverPrinter(CMSat::SATSolver* solver);
        ~BufferedSolverPrinter();

        void create(bool xOR, const std::vector<CMSat::Lit>& vars);
        void flush();
    private:
        std::vector<bool> clauseType;
        std::vector< std::vector<CMSat::Lit> > clause;
};

#endif //__BUFFEREDSOLVERPRINTER_H__
