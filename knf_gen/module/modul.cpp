#include "modul.h"

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <algorithm>

#include "clausecreator.h"

#include "../collector/solverprinter.h"
#include "../collector/logger.h"
#include "../collector/dimacsfileprinter.h"
#include "../collector/ttfileprinter.h"
#include "../collector/counter.h"

using std::vector;
using namespace CMSat;

Modul::Modul(unsigned bitWidth, unsigned inputCount, unsigned outputCount) {
    this->bitWidth = bitWidth;
    this->inputCount = inputCount;

    this->inputNum = 0;
    for (unsigned i = 0; i < inputCount; i++) {
        this->inputs.push_back(this->inputNum);
        this->inputWidth.push_back(bitWidth);
        this->inputNum += bitWidth;
    }
    this->outputNum = bitWidth * outputCount;

    this->start = this->inputNum;
}

Modul::~Modul() {
}

void Modul::setInputsBitWidth(...) {
    va_list ap;
    va_start(ap, this->inputCount);

    this->inputNum = 0;
    for (unsigned i = 0; i < inputCount; i++) {
        unsigned width = va_arg(ap, unsigned);
        this->inputs[i] = this->inputNum;
        this->inputWidth[i] = width;
        this->inputNum += width;
    }
    this->start = this->inputNum;

    va_end(ap);
}

unsigned Modul::getMaxVar() {
    return count()[STATS_MAXVAR];
}

unsigned Modul::getVarCount() {
    return count()[STATS_VARCOUNT];
}

unsigned Modul::getAdditionalVarCount() {
    return count()[STATS_VARCOUNT] - getInputNum();
}

unsigned Modul::getClauseCount() {
    return count()[STATS_CLAUSECOUNT];
}

void Modul::setInputs(const vector<unsigned>& inputs) {
    assert(inputs.size() == inputCount);
    this->inputs = inputs;

    unsigned max_index = std::distance(inputs.begin(), std::max_element(inputs.begin(), inputs.end()));
    setStart(inputs[max_index] + inputWidth[max_index]);
}

void Modul::setStart(unsigned start) {
    this->start = start;
    this->output = 0xFFFFFFFF - getOutputNum(); // TODO EVIL HACK
    this->output = start + getAdditionalVarCount() - getOutputNum();
}

void Modul::setOutput(unsigned output) {
    this->output = output;
}

unsigned Modul::getBitWidth() {
    return this->bitWidth;
}

vector<unsigned>& Modul::getInputs() {
    return this->inputs;
}

vector<unsigned>& Modul::getInputWidth() {
    return this->inputWidth;
}

unsigned Modul::getStart() {
    return this->start;
}

unsigned Modul::getOutput() {
    return this->output;
}

unsigned Modul::getInputNum() {
    return inputNum;
}

unsigned Modul::getOutputNum() {
    return outputNum;
}

unsigned Modul::append(SATSolver* solver) {
    //std::cout << "Start: " << start << " | Output: " << output << "\n";
    SolverPrinter printer(solver);
    create(&printer);
    return 0;
}

unsigned Modul::writeLog(const char* filename) {
    Logger logger("sha256.txt");
    create(&logger);
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

void Modul::createTrue(Collector* collector, unsigned out) {
    vector<Lit> clause(1, Lit(out, false));
    collector->create(false, clause);
}

void Modul::createFalse(Collector* collector, unsigned out) {
    vector<Lit> clause(1, Lit(out, true));
    collector->create(false, clause);
}

void Modul::createEQ(Collector* collector, unsigned out, unsigned in) {
    createXOR(collector, out, in);
}

void Modul::createNEQ(Collector* collector, unsigned out, unsigned in) {
    createXOR(collector, out, in, true);
}

void Modul::createAND(Collector* collector, unsigned out, unsigned in1, unsigned in2, bool invert) {
    ClauseCreator cc(collector);
    cc.setLiterals(3,     out,   in1,   in2);
    cc.printClause(3,  invert,     1, CC_DC);
    cc.printClause(3,  invert, CC_DC,     1);
    cc.printClause(3, !invert,     0,     0);
}

void Modul::createOR(Collector* collector, unsigned out, unsigned in1, unsigned in2, bool invert) {
    ClauseCreator cc(collector);
    cc.setLiterals(3,     out,   in1,   in2);
    cc.printClause(3, !invert,     0, CC_DC);
    cc.printClause(3, !invert, CC_DC,     0);
    cc.printClause(3,  invert,     1,     1);
}

void Modul::createXOR(Collector* collector, unsigned out, unsigned in1, bool invert) {
    vector<Lit> clause;
    clause.push_back(Lit(out, !invert));
    clause.push_back(Lit(in1, false));
    collector->create(true, clause);
}

void Modul::createXOR(Collector* collector, unsigned out, unsigned in1, unsigned in2, bool invert) {
    vector<Lit> clause;
    clause.push_back(Lit(out, !invert));
    clause.push_back(Lit(in1, false));
    clause.push_back(Lit(in2, false));
    collector->create(true, clause);
}

void Modul::createXOR(Collector* collector, unsigned out, unsigned in1, unsigned in2, unsigned in3, bool invert) {
    vector<Lit> clause;
    clause.push_back(Lit(out, !invert));
    clause.push_back(Lit(in1, false));
    clause.push_back(Lit(in2, false));
    clause.push_back(Lit(in3, false));
    collector->create(true, clause);
}

void Modul::createXOR(Collector* collector, unsigned out, unsigned in1, unsigned in2, unsigned in3, unsigned in4, bool invert) {
    vector<Lit> clause;
    clause.push_back(Lit(out, !invert));
    clause.push_back(Lit(in1, false));
    clause.push_back(Lit(in2, false));
    clause.push_back(Lit(in3, false));
    clause.push_back(Lit(in4, false));
    collector->create(true, clause);
}

unsigned* Modul::count() {
    unsigned* stats = getStats();
    if (stats[STATS_VARCOUNT] == 0) {
        Counter counter;
        create(&counter);
        stats[STATS_MAXVAR] = counter.getMaxVar();
        stats[STATS_VARCOUNT] = counter.getVarCount();
        stats[STATS_CLAUSECOUNT] = counter.getClauseCount();
    }
    return stats;
}
