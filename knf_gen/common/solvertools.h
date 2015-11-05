#ifndef __SOLVERTOOLS_H__
#define __SOLVERTOOLS_H__

#include <stdint.h>

#include "cryptominisat4/cryptominisat.h"

uint64_t solver_readInt(CMSat::SATSolver& solver, unsigned lsb, unsigned bitWidth);

void solver_writeInt(CMSat::SATSolver& solver, unsigned lsb, unsigned bitWidth, uint64_t value);

uint64_t solver_readInt_msb(CMSat::SATSolver& solver, unsigned msb, unsigned bitWidth);

void solver_writeInt_msb(CMSat::SATSolver& solver, unsigned msb, unsigned bitWidth, uint64_t value);

#endif //__SOLVERTOOLS_H__
