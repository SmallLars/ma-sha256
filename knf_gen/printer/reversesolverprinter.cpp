#include "reversesolverprinter.h"

using std::vector;
using namespace CMSat;

ReverseSolverPrinter::ReverseSolverPrinter(SATSolver* solver) : SolverPrinter(solver) {
}

ReverseSolverPrinter::~ReverseSolverPrinter() {
}

void ReverseSolverPrinter::create(bool xOR, const vector<Lit>& vars) {
    clauseType.push_back(xOR);
    clause.push_back(vars);
}

void ReverseSolverPrinter::flush() {
    while (!clauseType.empty()) {
        SolverPrinter::create(clauseType.back(), clause.back());
        clauseType.pop_back();
        clause.pop_back();
    }
}
