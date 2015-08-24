#include "add_ssig_32.h"

#include "ssig0_32.h"
#include "ssig1_32.h"
#include "add_32.h"
#include "clausecreator.h"

#include "../common/solvertools.h"

using std::vector;
using namespace CMSat;

unsigned Add_Ssig_32::stats[STATS_LENGTH];

Add_Ssig_32::Add_Ssig_32() : Modul(32, 2, 1) {
    inputs.push_back(0);
    inputs.push_back(32);
    
    start = 64;

    Ssig0_32 ssig0;
    Ssig1_32 ssig1;
    Add_32 adder;
    output = start + ssig0.getAdditionalVarCount() + ssig1.getAdditionalVarCount() + adder.getAdditionalVarCount() - 32;
}

Add_Ssig_32::~Add_Ssig_32() {
}

unsigned* Add_Ssig_32::getStats() {
    return stats;
}

void Add_Ssig_32::create(Printer* printer) {
    printer->newModul(11, "Add_Ssig_32", this);

    unsigned newvars = 0;
    vector<unsigned> subinputs;

    subinputs.clear();
    subinputs.push_back(inputs[0]);
    Ssig0_32 ssig0;
    ssig0.setInputs(subinputs);
    ssig0.setStart(start + newvars);
    ssig0.create(printer);
    newvars += ssig0.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(inputs[1]);
    Ssig1_32 ssig1;
    ssig1.setInputs(subinputs);
    ssig1.setStart(start + newvars);
    ssig1.create(printer);
    newvars += ssig1.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(ssig0.getOutput());
    subinputs.push_back(ssig1.getOutput());
    Add_32 adder;
    adder.setInputs(subinputs);
    adder.setStart(start + newvars);
    adder.create(printer);
    newvars += adder.getAdditionalVarCount();

#ifdef ADDITIONAL_CLAUSES
    ClauseCreator cc(printer);
    //                        93         156         157             46              48
    cc.setLiterals(5, start + 28, start + 91, start + 92, inputs[1]+ 13, inputs[1] + 15);
    cc.printClause(5,          1,      CC_DC,          0,             1,              1);
    cc.printClause(5,      CC_DC,          1,          0,             1,              1);
    cc.printClause(5,          0,      CC_DC,          1,             1,              0);
    cc.printClause(5,      CC_DC,          0,          1,             1,              0);
    cc.printClause(5,          0,      CC_DC,          1,             0,              1);
    cc.printClause(5,      CC_DC,          0,          1,             0,              1);
    cc.printClause(5,          1,      CC_DC,          0,             0,              0);
    cc.printClause(5,      CC_DC,          1,          0,             0,              0);
#endif
}

MU_TEST_C(Add_Ssig_32::test) {
    unsigned a[] = {0xabcdef98, 0x456af012};
    unsigned b[] = {0x651d8fa1, 0x456af012};

    for (unsigned t = 0; t < 2; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");

		uint32_t s0 = (a[t] >> 7 | a[t] << (32-7)) ^ (a[t] >> 18 | a[t] << (32-18)) ^ (a[t] >> 3);
		uint32_t s1 = (b[t] >> 17 | b[t] << (32-17)) ^ (b[t] >> 19 | b[t] << (32-19)) ^ (b[t] >> 10);
		uint32_t ausgabe = s0 + s1;

        solver_writeInt(solver,  0, 32, a[t]);
        solver_writeInt(solver, 32, 32, b[t]);

        Add_Ssig_32 adderSsig;
        adderSsig.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "ADDER_SSIG UNSAT");
        mu_assert(ausgabe == solver_readInt(solver, adderSsig.getOutput(), 32), "ADDER_SSIG failed");
    }
}
