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
    clauseCount++;
}
