#include "add_b1ch_32.h"

#include "const.h"
#include "bsig1_32.h"
#include "ch_32.h"
#include "add_32.h"

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
}

MU_TEST_C(Add_B1Ch_32::test) {
    unsigned a[] = {0xabcdef98};
    unsigned b[] = {0x651d8fa1};
    unsigned c[] = {0x456af012};

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");
        solver.set_num_threads(4);

		uint32_t S1 = (a[t] >> 6 | a[t] << (32-6)) ^ (a[t] >> 11 | a[t] << (32-11)) ^ (a[t] >> 25 | a[t] << (32-25));
		uint32_t ch = (a[t] & b[t]) ^ ((~a[t]) & c[t]);
		uint32_t ausgabe = S1 + ch;
        uint32_t result = 0;

        Const con(32, a[t]);
        con.setOutput(0);
        con.append(&solver);

        con.setValue(b[t]);
        con.setOutput(32);
        con.append(&solver);

        con.setValue(c[t]);
        con.setOutput(64);
        con.append(&solver);

        Add_B1Ch_32 adderB1Ch;
        adderB1Ch.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "ADDER_B1CH UNSAT");

        unsigned output = adderB1Ch.getOutput();
        for (unsigned i = output + 31; i >= output; i--) {
            result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - output));
        }

        mu_assert(ausgabe == result, "ADDER_B1CH failed");
    }
}
