#include "solverprinter.h"

#include "../common/clausetools.h"

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

    for (unsigned i = 0; i < vars.size(); i++) {
      if (vars[i].var() >= solver->nVars()) {
          //std::cout << "Added: " << (vars[i].var() - solver->nVars() + 1) << "\n";
          solver->new_vars(vars[i].var() - solver->nVars() + 1);
      }
    }

    if (xOR) {
        vector<unsigned> lits;
        for (unsigned i = 0; i < vars.size(); i++) lits.push_back(vars[i].var());
        solver->add_xor_clause(lits, !vars[0].sign());
        return;
    }

    solver->add_clause(vars);
}

void SolverPrinter::flush() {
}
