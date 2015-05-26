#include "modul.h"
#include <stdio.h>

Modul::Modul() {
}

Modul::~Modul() {
}

void Modul::setStart(unsigned start) {
    this->start = start;
}

void Modul::setInputs(const std::vector<unsigned>& inputs) {
    this->inputs = inputs;
}

unsigned Modul::append(std::string filename) {
    create(&Modul::createF);
    return 0;
}

unsigned Modul::append(CMSat::SATSolver* solver) {
    this->solver = solver;
    create(&Modul::createC);
    return 0;
}

void Modul::createF(bool xOR, const std::vector<signed>& vars) {
    if (xOR) {
        fprintf(stdout, "x-");
        for (unsigned i = 0; i < vars.size(); i++) fprintf(stdout, "%d ", abs(vars[i]));
        fprintf(stdout, "0\n");
    } else {
        for (unsigned i = 0; i < vars.size(); i++) fprintf(stdout, "%d ", vars[i]);
        fprintf(stdout, "0\n");
    }
}

void Modul::createC(bool xOR, const std::vector<signed>& vars) {
    if (xOR) {
        std::vector<unsigned> lits;
        for (unsigned i = 0; i < vars.size(); i++) lits.push_back(abs(vars[i]));
        solver->add_xor_clause(lits, false);
    } else {
        std::vector<CMSat::Lit> clause;
        for (unsigned i = 0; i < vars.size(); i++) clause.push_back(CMSat::Lit(abs(vars[i]), vars[i] > 0));
        solver->add_clause(clause);
    }
}
