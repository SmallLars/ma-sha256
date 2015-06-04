#include "counter.h"

using std::vector;
using namespace CMSat;

Counter::Counter() {
    varCount = 0;
    clauseCount = 0;
}

Counter::~Counter() {
}

unsigned Counter::getVarCount() {
    return varCount + 1;
}

unsigned Counter::getClauseCount() {
    return clauseCount;
}

void Counter::create(bool xOR, const vector<Lit>& vars) {
    for (unsigned i = 0; i < vars.size(); i++) {
        if (vars[i].var() > varCount) varCount = vars[i].var();
    }
    clauseCount++;
}
