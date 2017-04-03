#include "bufferedsolverprinter.h"

using std::vector;
using namespace CMSat;

BufferedSolverPrinter::BufferedSolverPrinter(SATSolver* solver) : SolverPrinter(solver) {
}

BufferedSolverPrinter::~BufferedSolverPrinter() {
}

void BufferedSolverPrinter::create(bool xOR, const vector<Lit>& vars) {
    clauseType.push_back(xOR);
    clause.push_back(vars);
}

void BufferedSolverPrinter::flush() {
    for (unsigned i = 0; i < clauseType.size(); i++) {
        SolverPrinter::create(clauseType[i], clause[i]);
    }
    clauseType.clear();
    clause.clear();
}
