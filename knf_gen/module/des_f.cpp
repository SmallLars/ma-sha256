#include "add_32.h"

#include "add_half_1.h"
#include "add_full_1.h"
#include "add_full_2.h"
#include "add_full_3.h"
#include "clausecreator.h"

#include "../common/solvertools.h"

using std::vector;
using namespace CMSat;

unsigned Des_F::stats[STATS_LENGTH];

Des_F::Des_F(uint round) : Modul(32, 2, 1) {
    setInputsBitWidth(32, 56);
    this->round = round;
    output = 136;
}

Des_F::~Des_F() {
}

unsigned* Des_F::getStats() {
    return stats;
}

void Des_F::create(Printer* printer) {
    printer->newModul(10, "Des_F", this);

    // E exploding
    // K_1 ... generation

    for(uint i = 0; i < 48; i++){
        createXOR(printer, output, inputs[0], inputs[1], inputs[2]);
    }
}

MU_TEST_C(Des_F::test) {
    unsigned a[] = {1234, 5, 0x80000000, 1, 0xFFFFFFFF, 0x2, 0xFFFFFFFF, 0x1, 0xFFFFFFFF, 0x0};
    unsigned b[] = {1235, 6, 1, 0x80000000, 0x2, 0xFFFFFFFF, 0x1, 0xFFFFFFFF, 0x0, 0xFFFFFFFF};

    for (unsigned t = 0; t < 10; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");

        uint32_t ausgabe = a[t] + b[t];

        solver_writeInt(solver,  0, 32, a[t]);
        solver_writeInt(solver, 32, 32, b[t]);

        Des_F adder;
        adder.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "Adder UNSAT");
        mu_assert(ausgabe == solver_readInt(solver, 95, 32), "Adder failed");
    }
}
