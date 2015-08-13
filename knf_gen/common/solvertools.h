#ifndef __SOLVERTOOLS_H__
#define __SOLVERTOOLS_H__

#include <stdint.h>

#include "cryptominisat4/cryptominisat.h"

uint32_t solver_readInt(CMSat::SATSolver& solver, unsigned lsb, unsigned bitWidth);

#endif //__SOLVERTOOLS_H__
