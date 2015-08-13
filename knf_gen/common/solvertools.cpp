#include "solvertools.h"

using namespace CMSat;

uint32_t solver_readInt(SATSolver& solver, unsigned lsb, unsigned bitWidth) {
    uint32_t result = 0;
    for (unsigned b = lsb; b < lsb + bitWidth; b++) {
        result |= ((solver.get_model()[b] == l_True? 1 : 0) << (b - lsb));
    }
    return result;
}
