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
    printf("Datei erweitern\n");
}

void Modul::createC(bool xOR, const std::vector<CMSat::Lit>& vars) {
    std::vector<unsigned> lits;
    for (unsigned i = 0; i < vars.size(); i++) lits.push_back(vars[i].toInt());
    solver->add_xor_clause(lits, false);
}
