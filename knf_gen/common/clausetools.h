#ifndef __CLAUSETOOLS_H__
#define __CLAUSETOOLS_H__

#include <iostream>
#include <vector>
#include <set>
#include <map>

#include "cryptominisat4/cryptominisat.h"

struct compareClause {
    bool operator() (const std::vector<CMSat::Lit>& lhs, const std::vector<CMSat::Lit>& rhs) const;
};

void printClause(std::ostream& out, const std::vector<CMSat::Lit>& clause, bool revert = false);

void createLookup(std::map< CMSat::Lit, std::vector< const std::vector<CMSat::Lit>* > >& lookup_table, const std::set<std::vector<CMSat::Lit>, compareClause>& clause_pool);

bool hasSubClause(const std::vector<CMSat::Lit>& clause, std::map< CMSat::Lit, std::vector< const std::vector<CMSat::Lit>* > >& lookup_table);

std::vector< std::vector<CMSat::Lit> > convertXOR(const std::vector<CMSat::Lit>& vars);

#endif //__CLAUSETOOLS_H__
