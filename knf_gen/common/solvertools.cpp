#include "solvertools.h"

#include "../module/const.h"

using namespace CMSat;

uint64_t solver_readInt(SATSolver& solver, unsigned lsb, unsigned bitWidth) {
    uint64_t result = 0;
    for (unsigned b = 0; b < bitWidth; b++) {
        result |= ((uint64_t)(solver.get_model()[lsb + b] == l_True? 1 : 0) << b);
    }
    return result;
}

void solver_writeInt(SATSolver& solver, unsigned lsb, unsigned bitWidth, uint64_t value) {
    Const con(bitWidth, value);
    con.setOutput(lsb);
    con.append(&solver);
}

uint64_t solver_readInt_msb(CMSat::SATSolver& solver, unsigned msb, unsigned bitWidth) {
    uint64_t result = 0;
    for (unsigned b = 0; b < bitWidth; b++) {
        result |= ((uint64_t)(solver.get_model()[msb + b] == l_True? 1 : 0) << (bitWidth - b - 1));
    }
    return result;
}

void solver_writeInt_msb(CMSat::SATSolver& solver, unsigned msb, unsigned bitWidth, uint64_t value) {
    Const con(bitWidth, value, false);
    con.setOutput(msb);
    con.append(&solver);
}
