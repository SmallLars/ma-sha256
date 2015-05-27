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

void Modul::createF(bool xOR, const std::vector<CMSat::Lit>& vars) {
    if (xOR) {
        fprintf(stdout, "x-");
        for (unsigned i = 0; i < vars.size(); i++) fprintf(stdout, "%d ", vars[i].var());
        fprintf(stdout, "0\n");
    } else {
        for (unsigned i = 0; i < vars.size(); i++) {
            if (vars[i].sign()) fprintf(stdout, "-");
            fprintf(stdout, "%d ", vars[i].var());
        }
        fprintf(stdout, "0\n");
    }
}

void Modul::createC(bool xOR, const std::vector<CMSat::Lit>& clause) {
    if (xOR) {
        std::vector<unsigned> lits;
        for (unsigned i = 0; i < clause.size(); i++) lits.push_back(clause[i].var());
        solver->add_xor_clause(lits, false);
    } else {
        solver->add_clause(clause);
    }
}
