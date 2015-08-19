#include "add_b0maj_32.h"

#include "bsig0_32.h"
#include "maj_32.h"
#include "add_32.h"

#include "../common/solvertools.h"

using std::vector;
using namespace CMSat;

unsigned Add_B0Maj_32::stats[STATS_LENGTH];

Add_B0Maj_32::Add_B0Maj_32() : Modul(32, 3, 1) {
    inputs.push_back(0);
    inputs.push_back(32);
    inputs.push_back(64);
    
    start = 96;

    Bsig0_32 bsig0;
    Maj_32 maj;
    Add_32 adder;
    output = start + bsig0.getAdditionalVarCount() + maj.getAdditionalVarCount() + adder.getAdditionalVarCount() - 32;
}

Add_B0Maj_32::~Add_B0Maj_32() {
}

unsigned* Add_B0Maj_32::getStats() {
    return stats;
}

void Add_B0Maj_32::create(Printer* printer) {
    printer->newModul(11, "Add_B0Maj_32", this);

    unsigned newvars = 0;
    vector<unsigned> subinputs;

    subinputs.clear();
    subinputs.push_back(inputs[0]);
    Bsig0_32 bsig0;
    bsig0.setInputs(subinputs);
    bsig0.setStart(start + newvars);
    bsig0.create(printer);
    newvars += bsig0.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(inputs[0]);
    subinputs.push_back(inputs[1]);
    subinputs.push_back(inputs[2]);
    Maj_32 maj;
    maj.setInputs(subinputs);
    maj.setStart(start + newvars);
    maj.create(printer);
    newvars += maj.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(bsig0.getOutput());
    subinputs.push_back(maj.getOutput());
    Add_32 adder;
    adder.setInputs(subinputs);
    adder.setStart(start + newvars);
    adder.create(printer);
    newvars += adder.getAdditionalVarCount();
}

MU_TEST_C(Add_B0Maj_32::test) {
    unsigned a[] = {0xabcdef98};
    unsigned b[] = {0x651d8fa1};
    unsigned c[] = {0x456af012};

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");

		uint32_t S0 = (a[t] >> 2 | a[t] << (32-2)) ^ (a[t] >> 13 | a[t] << (32-13)) ^ (a[t] >> 22 | a[t] << (32-22));
		uint32_t maj = (a[t] & b[t]) ^ (a[t] & c[t]) ^ (b[t] & c[t]);
		uint32_t ausgabe = S0 + maj;

        solver_writeInt(solver,  0, 32, a[t]);
        solver_writeInt(solver, 32, 32, b[t]);
        solver_writeInt(solver, 64, 32, c[t]);

        Add_B0Maj_32 adderB0Maj;
        adderB0Maj.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "ADDER_B0MAJ UNSAT");
        mu_assert(ausgabe == solver_readInt(solver, adderB0Maj.getOutput(), 32), "ADDER_B0MAJ failed");
    }
}
