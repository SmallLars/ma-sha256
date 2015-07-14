#include "add_b0maj_32.h"

#include "const.h"
#include "bsig0_32.h"
#include "maj_32.h"
#include "add_32.h"

using std::vector;
using namespace CMSat;

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

void Add_B0Maj_32::create(Printer* printer) {
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
        solver.set_num_threads(4);

		uint32_t S0 = (a[t] >> 2 | a[t] << (32-2)) ^ (a[t] >> 13 | a[t] << (32-13)) ^ (a[t] >> 22 | a[t] << (32-22));
		uint32_t maj = (a[t] & b[t]) ^ (a[t] & c[t]) ^ (b[t] & c[t]);
		uint32_t ausgabe = S0 + maj;
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

        Add_B0Maj_32 adderB0Maj;
        adderB0Maj.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "ADDER_B0MAJ UNSAT");

        unsigned output = adderB0Maj.getOutput();
        for (unsigned i = output + 31; i >= output; i--) {
            result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - output));
        }

        mu_assert(ausgabe == result, "ADDER_B0MAJ failed");
    }
}
