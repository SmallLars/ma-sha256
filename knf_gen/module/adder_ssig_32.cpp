#include "adder_ssig_32.h"

#include "const_32.h"
#include "ssig0_32.h"
#include "ssig1_32.h"
#include "adder_32.h"

using std::vector;
using namespace CMSat;

Adder_Ssig_32::Adder_Ssig_32() : Modul(32, 2, 1) {
    inputs.push_back(0);
    inputs.push_back(32);
    
    start = 64;

    Ssig0_32 ssig0;
    Ssig1_32 ssig1;
    Adder_32 adder;
    output = start + ssig0.getAdditionalVarCount() + ssig1.getAdditionalVarCount() + adder.getAdditionalVarCount() - 32;
}

Adder_Ssig_32::~Adder_Ssig_32() {
}

void Adder_Ssig_32::create(Printer* printer) {
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
    Adder_32 adder;
    adder.setInputs(subinputs);
    adder.setStart(start + newvars);
    adder.create(printer);
    newvars += adder.getAdditionalVarCount();
}

MU_TEST_C(Adder_Ssig_32::test) {
    unsigned a[] = {0xabcdef98, 0x456af012};
    unsigned b[] = {0x651d8fa1, 0x456af012};

    for (unsigned t = 0; t < 2; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");
        solver.set_num_threads(4);

		uint32_t s0 = (a[t] >> 7 | a[t] << (32-7)) ^ (a[t] >> 18 | a[t] << (32-18)) ^ (a[t] >> 3);
		uint32_t s1 = (b[t] >> 17 | b[t] << (32-17)) ^ (b[t] >> 19 | b[t] << (32-19)) ^ (b[t] >> 10);
		uint32_t ausgabe = s0 + s1;
        uint32_t result = 0;

        Const_32 ca(a[t]);
        ca.setOutput(0);
        ca.append(&solver);

        Const_32 cb(b[t]);
        cb.setOutput(32);
        cb.append(&solver);

        Adder_Ssig_32 adderSsig;
        adderSsig.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "ADDER_SSIG UNSAT");

        unsigned output = adderSsig.getOutput();
        for (unsigned i = output + 31; i >= output; i--) {
            result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - output));
        }

        mu_assert(ausgabe == result, "ADDER_SSIG failed");
    }
}
