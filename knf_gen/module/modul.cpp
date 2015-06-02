#include "modul.h"

#include <stdio.h>
#include <assert.h>
#include <iostream>

using std::vector;
using namespace CMSat;

Modul::Modul(unsigned inputCount) {
    this->inputCount = inputCount;
}

Modul::~Modul() {
}

unsigned Modul::getVarCount() {
    return 32 * inputCount + getAdditionalVarCount();
}

void Modul::setStart(unsigned start) {
    this->start = start;
}

void Modul::setInputs(const vector<unsigned>& inputs) {
    assert(inputs.size() == inputCount);
    this->inputs = inputs;
}

unsigned Modul::append(SATSolver* solver) {
    this->solver = solver;
    // TODO var count checken und bei Bedarf erhöhen
    create(&Modul::createSolver);
    return 0;
}

unsigned Modul::appendDimacs(const char* filename) {
    outputFile.open(filename, std::ios::out | std::ios::app);
    if (outputFile.tellp() == 0) {
        outputFile << "p cnf " << getVarCount() << " " << getClauseCount() << "\n";
    }
    create(&Modul::createDimacs);
    outputFile.close();
    return 0;
}

unsigned Modul::appendTT(const char* filename) {
    outputFile.open(filename, std::ios::out | std::ios::app);
    if (outputFile.tellp() == 0) {
        outputFile << ".i " << getVarCount() << "\n.o 1\n.type r\n";
    }
    create(&Modul::createTT);
    outputFile.close();
    return 0;
}

void Modul::createSolver(bool xOR, const vector<Lit>& clause) {
    if (xOR) {
        vector<unsigned> lits;
        for (unsigned i = 0; i < clause.size(); i++) lits.push_back(clause[i].var());
        solver->add_xor_clause(lits, false);
    } else {
        solver->add_clause(clause);
    }
}

void Modul::createDimacs(bool xOR, const vector<Lit>& vars) {
    if (xOR) {
        outputFile << "x-";
        for (unsigned i = 0; i < vars.size(); i++) outputFile << vars[i].var() << " ";
        outputFile << "0\n";
    } else {
        for (unsigned i = 0; i < vars.size(); i++) {
            if (vars[i].sign()) outputFile << "-";
            outputFile << vars[i].var() << " ";
        }
        outputFile << "0\n";
    }
}

void Modul::createTT(bool xOR, const vector<Lit>& vars) {
    // 0--1--|0
    if (xOR) {
        outputFile << "x-";
        for (unsigned i = 0; i < vars.size(); i++) outputFile << vars[i].var() << " ";
        outputFile << "0\n";
    } else {
        for (unsigned i = 0; i < vars.size(); i++) {
            if (vars[i].sign()) outputFile << "-";
            outputFile << vars[i].var() << " ";
        }
        outputFile << "0\n";
    }
}
