#include "solverprinter.h"

using std::vector;
using namespace CMSat;

SolverPrinter::SolverPrinter(SATSolver* solver) {
    this->solver = solver;
}

SolverPrinter::~SolverPrinter() {
}

void SolverPrinter::create(bool xOR, const vector<Lit>& vars) {
#ifndef XOR_SUPPORT
    if (xOR) {
        vector< vector<Lit> > clauses = convertXOR(vars);
        for (unsigned i = 0; i < clauses.size(); i++) {
             create(false, clauses[i]);
        }
        return;
    }
#endif

    if (xOR) {
        vector<unsigned> lits;
        for (unsigned i = 0; i < vars.size(); i++) lits.push_back(vars[i].var());
        solver->add_xor_clause(lits, false);
        return;
    }

    solver->add_clause(vars);
}
