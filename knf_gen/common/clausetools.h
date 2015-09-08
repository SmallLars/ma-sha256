#ifndef __CLAUSETOOLS_H__
#define __CLAUSETOOLS_H__

#include <iostream>
#include <vector>

#include "cryptominisat4/cryptominisat.h"

struct compareClause {
    bool operator() (const std::vector<CMSat::Lit>& lhs, const std::vector<CMSat::Lit>& rhs) const;
};

void printClause(std::ostream& out, const std::vector<CMSat::Lit>& clause, bool revert = false);

#endif //__CLAUSETOOLS_H__
