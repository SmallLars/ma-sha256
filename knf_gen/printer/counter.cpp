#include "counter.h"

using std::vector;
using std::set;
using namespace CMSat;

Counter::Counter() {
    clauseCount = 0;
}

Counter::~Counter() {
}

unsigned Counter::getMaxVar() {
    return *(varSet.rbegin()) + 1;
}

unsigned Counter::getVarCount() {
    return varSet.size();
}

unsigned Counter::getClauseCount() {
    return clauseCount;
}

void Counter::create(bool xOR, const vector<Lit>& vars) {
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
        varSet.insert(vars[i].var());
    }
    clauseCount++;
}
