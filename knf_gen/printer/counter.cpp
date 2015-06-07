#include "counter.h"

#ifndef XOR_SUPPORT
#include <cmath>
#endif

using std::vector;
using std::set;
using namespace CMSat;

Counter::Counter() {
    clauseCount = 0;
}

Counter::~Counter() {
}

unsigned Counter::getMaxVar() {
    return *(varSet.rbegin());
}

unsigned Counter::getVarCount() {
    return varSet.size();
}

unsigned Counter::getClauseCount() {
    return clauseCount;
}

void Counter::create(bool xOR, const vector<Lit>& vars) {
    for (unsigned i = 0; i < vars.size(); i++) {
        varSet.insert(vars[i].var());
    }
#ifdef XOR_SUPPORT
    clauseCount++;
#else
    clauseCount += pow(2, vars.size() - 1);
#endif
}
