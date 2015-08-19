#include "const.h"

#include "../common/solvertools.h"

using std::vector;
using namespace CMSat;

unsigned Const::stats[STATS_LENGTH];

Const::Const(unsigned bitWidth, uint32_t value) : Modul(bitWidth, 0, 1) {
    this->value = value;
    output = 0;
}

Const::~Const() {
}

void Const::setValue(uint32_t value) {
    this->value = value;
}

unsigned* Const::getStats() {
    return stats;
}

void Const::create(Printer* printer) {
    printer->newModul(10, "Const", this);
/*
    for (unsigned i = getBitWidth(); i > 0; i--) {
        vector<Lit> clause;
        clause.push_back(Lit(output + i - 1, !(value >> (i - 1) & 1)));
        printer->create(false, clause);
    }
*/
    for (unsigned i = 0; i < getBitWidth(); i++) {
        vector<Lit> clause;
        clause.push_back(Lit(output + i, !(value >> i & 1)));
        printer->create(false, clause);
    }
}

MU_TEST_C(Const::test) {
    unsigned value[] = {1234, 5, 0x80000000, 1, 0xFFFFFFFF,   2, 0xFFFFFF, 0x1, 0xFFFF, 0x0};
    unsigned width[] = {  32, 3,         32, 1,         32,   2,       24,   2,     16,   3};

    for (unsigned t = 0; t < 10; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");

        Const c(width[t], value[t]);
        c.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "Adder UNSAT");
        mu_assert(value[t] == solver_readInt(solver, 0, width[t]), "Adder failed");
    }
}
