#include "modul.h"

#include <stdio.h>
#include <assert.h>
#include <algorithm>

#include "clausecreator.h"

#include "../printer/solverprinter.h"
#include "../printer/dimacsfileprinter.h"
#include "../printer/ttfileprinter.h"
#include "../printer/counter.h"

using std::vector;
using namespace CMSat;

Modul::Modul(unsigned bitWidth, unsigned inputCount, unsigned outputCount) {
    this->bitWidth = bitWidth;
    this->inputCount = inputCount;
    this->outputCount = outputCount;
}

Modul::~Modul() {
}

unsigned Modul::getMaxVar() {
    Counter counter;
    create(&counter);
    return counter.getMaxVar();
}

unsigned Modul::getVarCount() {
    Counter counter;
    create(&counter);
    return counter.getVarCount();
}

unsigned Modul::getAdditionalVarCount() {
    Counter counter;
    create(&counter);
    return counter.getVarCount() - (inputCount * bitWidth);
}

unsigned Modul::getClauseCount() {
    Counter counter;
    create(&counter);
    return counter.getClauseCount();
}

void Modul::setInputs(const vector<unsigned>& inputs) {
    assert(inputs.size() == inputCount);
    this->inputs = inputs;
    setStart(*std::max_element(inputs.begin(), inputs.end()) + bitWidth);
}

void Modul::setStart(unsigned start) {
    this->start = start;
    this->output = 0xFFFFFFFF - (outputCount * bitWidth); // TODO EVIL HACK
    this->output = start + getAdditionalVarCount() - (outputCount * bitWidth);
}

void Modul::setOutput(unsigned output) {
    this->output = output;
}

unsigned Modul::getInputNum() {
    return this->bitWidth * this->inputCount;
}

unsigned Modul::getOutput() {
    return this->output;
}

unsigned Modul::getOutputNum() {
    return this->bitWidth * this->outputCount;
}

unsigned Modul::append(SATSolver* solver) {
    //std::cout << "Start: " << start << " | Output: " << output << "\n";
    unsigned maxVar = getMaxVar();
    if (maxVar >= solver->nVars()) {
        //std::cout << "Added: " << (maxVar - solver->nVars() + 1) << "\n";
        solver->new_vars(maxVar - solver->nVars() + 1);
    }
    SolverPrinter printer(solver);
    create(&printer);
    return 0;
}

unsigned Modul::writeDimacs(const char* filename) {
    DimacsFilePrinter printer(filename, getVarCount(), getClauseCount());
    create(&printer);
    return 0;
}

unsigned Modul::writeTT(const char* filename) {
    TTFilePrinter printer(filename, getVarCount());
    create(&printer);
    return 0;
}

void Modul::createTrue(Printer* printer, unsigned out) {
    vector<Lit> clause;
    clause.push_back(Lit(out, false));
    printer->create(false, clause);
}

void Modul::createFalse(Printer* printer, unsigned out) {
    vector<Lit> clause;
    clause.push_back(Lit(out, true));
    printer->create(false, clause);
}

void Modul::createEQ(Printer* printer, unsigned out, unsigned in) {
    vector<Lit> clause;
    clause.push_back(Lit(out, false));
    clause.push_back(Lit(in, false));
    printer->create(true, clause);
}

void Modul::createNEQ(Printer* printer, unsigned out, unsigned in) {
    vector<Lit> clause;
    clause.push_back(Lit(out, true));
    clause.push_back(Lit(in, false));
    printer->create(true, clause);
}

void Modul::createAND(Printer* printer, unsigned out, unsigned in1, unsigned in2) {
    ClauseCreator cc(printer);
    cc.setLiterals(3,   out,   in1,   in2);
    cc.printClause(3,     1,     0,     0);
    cc.printClause(3,     0,     1, CC_DC);
    cc.printClause(3,     0, CC_DC,     1);
}

void Modul::createXOR(Printer* printer, unsigned out, unsigned in1, unsigned in2, bool invert) {
    vector<Lit> clause;
    clause.push_back(Lit(out, invert));
    clause.push_back(Lit(in1, false));
    clause.push_back(Lit(in2, false));
    printer->create(true, clause);
}

void Modul::createOR(Printer* printer, unsigned out, unsigned in1, unsigned in2) {
    ClauseCreator cc(printer);
    cc.setLiterals(3,   out,   in1,   in2);
    cc.printClause(3,     0,     1,     1);
    cc.printClause(3,     1,     0, CC_DC);
    cc.printClause(3,     1, CC_DC,     0);
}

void Modul::createXOR(Printer* printer, unsigned out, unsigned in1, unsigned in2, unsigned in3, bool invert) {
    vector<Lit> clause;
    clause.push_back(Lit(out, invert));
    clause.push_back(Lit(in1, false));
    clause.push_back(Lit(in2, false));
    clause.push_back(Lit(in3, false));
    printer->create(true, clause);
}
