#ifndef __CLAUSEPRINTER_H__
#define __CLAUSEPRINTER_H__

#include <iostream>
#include <vector>

#include "cryptominisat4/cryptominisat.h"

void printClause(std::ostream& out, const std::vector<CMSat::Lit>& clause);

#endif //__CLAUSEPRINTER_H__
