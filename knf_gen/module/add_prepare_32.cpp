#include "add_prepare_32.h"

#include "add_32.h"
#include "add_ssig_32.h"

#include "../common/solvertools.h"

using std::vector;
using namespace CMSat;

unsigned Add_Prepare_32::stats[STATS_LENGTH];

Add_Prepare_32::Add_Prepare_32() : Modul(32, 4, 1) {
    Add_32 adder;
    Add_Ssig_32 adderSsig;
    output = start + adderSsig.getAdditionalVarCount() + 2 * adder.getAdditionalVarCount() - 32;
}

Add_Prepare_32::~Add_Prepare_32() {
}

unsigned* Add_Prepare_32::getStats() {
    return stats;
}

void Add_Prepare_32::create(Printer* printer) {
    printer->newModul(12, "Add_Prepare_32", this);

    unsigned newvars = 0;
    vector<unsigned> subinputs;

    subinputs.clear();
    subinputs.push_back(inputs[1]);
    subinputs.push_back(inputs[3]);
    Add_Ssig_32 adder1;
    adder1.setInputs(subinputs);
    adder1.setStart(start + newvars);
    adder1.create(printer);
    newvars += adder1.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(inputs[0]);
    subinputs.push_back(inputs[2]);
    Add_32 adder2;
    adder2.setInputs(subinputs);
    adder2.setStart(start + newvars);
    adder2.create(printer);
    newvars += adder2.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(adder1.getOutput());
    subinputs.push_back(adder2.getOutput());
    Add_32 adder3;
    adder3.setInputs(subinputs);
    adder3.setStart(start + newvars);
    adder3.create(printer);
    newvars += adder3.getAdditionalVarCount();
}

MU_TEST_C(Add_Prepare_32::test) {
    unsigned a[] = {0xabcdef98};
    unsigned b[] = {0x651d8fa1};
    unsigned c[] = {0x456af012};
    unsigned d[] = {0x007fa4c5};

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");

		uint32_t s0 = (b[t] >> 7 | b[t] << (32-7)) ^ (b[t] >> 18 | b[t] << (32-18)) ^ (b[t] >> 3);
		uint32_t s1 = (d[t] >> 17 | d[t] << (32-17)) ^ (d[t] >> 19 | d[t] << (32-19)) ^ (d[t] >> 10);
		uint32_t ausgabe = a[t] + s0 + c[t] + s1;

        solver_writeInt(solver,  0, 32, a[t]);
        solver_writeInt(solver, 32, 32, b[t]);
        solver_writeInt(solver, 64, 32, c[t]);
        solver_writeInt(solver, 96, 32, d[t]);

        Add_Prepare_32 adderPrepare;
        adderPrepare.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "ADDER_PREPARE UNSAT");
        mu_assert(ausgabe == solver_readInt(solver, adderPrepare.getOutput(), 32), "ADDER_PREPARE failed");
    }
}
