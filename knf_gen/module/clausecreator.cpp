#include "clausecreator.h"

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

using std::vector;
using namespace CMSat;

ClauseCreator::ClauseCreator(Collector* collector) {
    this->collector = collector;
}

ClauseCreator::~ClauseCreator() {
}

Collector* ClauseCreator::getCollector() {
    return collector;
}

void ClauseCreator::setLiterals(unsigned n_args, ...) {
    literals.clear();

    va_list ap;
    va_start(ap, n_args);
    for (unsigned i = 0; i < n_args; i++) {
        literals.push_back(va_arg(ap, unsigned));
    }
    va_end(ap);
}

void ClauseCreator::addLiterals(unsigned n_args, ...) {
    va_list ap;
    va_start(ap, n_args);
    for (unsigned i = 0; i < n_args; i++) {
        literals.push_back(va_arg(ap, unsigned));
    }
    va_end(ap);
}

void ClauseCreator::printClause(unsigned n_args, ...) {
    assert(literals.size() == n_args);
    vector<Lit> clause;

    va_list ap;
    va_start(ap, n_args);
    for (unsigned i = 0; i < n_args; i++) {
        unsigned value = va_arg(ap, unsigned);
        if (value < 2) clause.push_back(Lit(literals[i], value == 0 ? true : false));
    }
    va_end(ap);
    collector->create(false, clause);
}
