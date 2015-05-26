#include "modul.h"
#include <stdio.h>

Modul::Modul() {
    inputs.clear();
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

unsigned Modul::append(CMSat::SATSolver& solver) {
    create(&Modul::createC);
    return 0;
}

void Modul::createF(bool xOR, const std::vector<signed>& vars) {
    printf("Datei erweitern\n");
}

void Modul::createC(bool xOR, const std::vector<signed>& vars) {
    printf("Solver erweitern\n");
}
