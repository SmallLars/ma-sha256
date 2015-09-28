#include "add_b1ch_32.h"

#include "bsig1_32.h"
#include "ch_32.h"
#include "add_32.h"
#include "clausecreator.h"

#include "../common/solvertools.h"

using std::vector;
using namespace CMSat;

unsigned Add_B1Ch_32::stats[STATS_LENGTH];

Add_B1Ch_32::Add_B1Ch_32() : Modul(32, 3, 1) {
    inputs.push_back(0);
    inputs.push_back(32);
    inputs.push_back(64);
    
    start = 96;

    Bsig1_32 bsig1;
    Ch_32 ch;
    Add_32 adder;
    output = start + bsig1.getAdditionalVarCount() + ch.getAdditionalVarCount() + adder.getAdditionalVarCount() - 32;
}

Add_B1Ch_32::~Add_B1Ch_32() {
}

unsigned* Add_B1Ch_32::getStats() {
    return stats;
}

void Add_B1Ch_32::create(Printer* printer) {
    printer->newModul(11, "Add_B1Ch_32", this);

    unsigned newvars = 0;
    vector<unsigned> subinputs;

    subinputs.clear();
    subinputs.push_back(inputs[0]);
    Bsig1_32 bsig1;
    bsig1.setInputs(subinputs);
    bsig1.setStart(start + newvars);
    bsig1.create(printer);
    newvars += bsig1.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(inputs[0]);
    subinputs.push_back(inputs[1]);
    subinputs.push_back(inputs[2]);
    Ch_32 ch;
    ch.setInputs(subinputs);
    ch.setStart(start + newvars);
    ch.create(printer);
    newvars += ch.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(bsig1.getOutput());
    subinputs.push_back(ch.getOutput());
    Add_32 adder;
    adder.setInputs(subinputs);
    adder.setStart(start + newvars);
    adder.create(printer);
    newvars += adder.getAdditionalVarCount();

#ifdef ADDITIONAL_CLAUSES
    ClauseCreator cc(printer);
    for (unsigned i = 0; i < 31; i++) {
        cc.setLiterals(6, inputs[0] + i, inputs[1] + i, inputs[2] + i, start + i, start + 63 + i, start + 64 + i);
        cc.printClause(6,             0,             1,         CC_DC,         1,          CC_DC,              0);
        cc.printClause(6,             1,         CC_DC,             1,         1,          CC_DC,              0);
        cc.printClause(6,             0,             0,         CC_DC,         0,          CC_DC,              1);
        cc.printClause(6,             1,         CC_DC,             0,         0,          CC_DC,              1);
        cc.printClause(6,             0,             1,         CC_DC,     CC_DC,              1,              0);
        cc.printClause(6,             1,         CC_DC,             1,     CC_DC,              1,              0);
        if (i == 0) continue;
        cc.printClause(6,             0,             0,         CC_DC,     CC_DC,              0,              1);
        cc.printClause(6,             1,         CC_DC,             0,     CC_DC,              0,              1);
    }

    for (unsigned i = 0; i < 32; i++) {
        cc.setLiterals(6, inputs[0] + i, inputs[1] + i, inputs[2] + i, start + i, start + 63 + i, output + i);
        cc.printClause(6,             1,         CC_DC,             1,         0,              1,          1);
        cc.printClause(6,             0,             1,         CC_DC,         0,              1,          1);
    }

    for (unsigned i = 0; i < 32; i++) {
        cc.setLiterals(6, inputs[0] + i, inputs[1] + i, inputs[2] + i, start + i, start + 32 + i, start + 63 + i);
        cc.printClause(6,             1,         CC_DC,             1,         0,              0,              1);
        cc.printClause(6,             1,         CC_DC,             0,         1,              1,              0);
        cc.printClause(6,             0,             1,         CC_DC,         0,              0,              1);
        cc.printClause(6,             0,             0,         CC_DC,         1,              1,              0);
    }

    for (unsigned i = 0; i < 31; i++) {
        cc.setLiterals(7, inputs[0] + i, inputs[1] + i, inputs[2] + i, start + i, start + 32 + i, start + 63 + i, start + 64 + i);
        cc.printClause(7,             1,         CC_DC,             1,         0,              1,              1,              0);
        cc.printClause(7,             0,             1,         CC_DC,         0,              1,              1,              0);
    }
#endif
}

MU_TEST_C(Add_B1Ch_32::test) {
    unsigned a[] = {0xabcdef98};
    unsigned b[] = {0x651d8fa1};
    unsigned c[] = {0x456af012};

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");

		uint32_t S1 = (a[t] >> 6 | a[t] << (32-6)) ^ (a[t] >> 11 | a[t] << (32-11)) ^ (a[t] >> 25 | a[t] << (32-25));
		uint32_t ch = (a[t] & b[t]) ^ ((~a[t]) & c[t]);
		uint32_t ausgabe = S1 + ch;

        solver_writeInt(solver,  0, 32, a[t]);
        solver_writeInt(solver, 32, 32, b[t]);
        solver_writeInt(solver, 64, 32, c[t]);

        Add_B1Ch_32 adderB1Ch;
        adderB1Ch.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "ADDER_B1CH UNSAT");
        mu_assert(ausgabe == solver_readInt(solver, adderB1Ch.getOutput(), 32), "ADDER_B1CH failed");
    }
}
