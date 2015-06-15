#include "extension_32.h"

#include "const_32.h"
#include "adder_32.h"
#include "adder_ssig_32.h"

using std::vector;
using namespace CMSat;

Extension_32::Extension_32() : Modul(32, 4, 1) {
    inputs.push_back(0);
    inputs.push_back(32);
    inputs.push_back(64);
    inputs.push_back(96);
    
    start = 128;

    Adder_32 adder;
    Adder_Ssig_32 adderSsig;
    output = start + adderSsig.getAdditionalVarCount() + 2 * adder.getAdditionalVarCount() - 32;
}

Extension_32::~Extension_32() {
}

void Extension_32::create(Printer* printer) {
    unsigned newvars = 0;
    vector<unsigned> subinputs;

    subinputs.clear();
    subinputs.push_back(inputs[1]);
    subinputs.push_back(inputs[3]);
    Adder_Ssig_32 adder1;
    adder1.setInputs(subinputs);
    adder1.setStart(start + newvars);
    adder1.create(printer);
    newvars += adder1.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(inputs[0]);
    subinputs.push_back(inputs[2]);
    Adder_32 adder2;
    adder2.setInputs(subinputs);
    adder2.setStart(start + newvars);
    adder2.create(printer);
    newvars += adder2.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(adder1.getOutput());
    subinputs.push_back(adder2.getOutput());
    Adder_32 adder3;
    adder3.setInputs(subinputs);
    adder3.setStart(start + newvars);
    adder3.create(printer);
    newvars += adder3.getAdditionalVarCount();
}

MU_TEST_C(Extension_32::test) {
    unsigned a[] = {0xabcdef98};
    unsigned b[] = {0x651d8fa1};
    unsigned c[] = {0x456af012};
    unsigned d[] = {0x007fa4c5};

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");
        solver.set_num_threads(4);

		uint32_t s0 = (b[t] >> 7 | b[t] << (32-7)) ^ (b[t] >> 18 | b[t] << (32-18)) ^ (b[t] >> 3);
		uint32_t s1 = (d[t] >> 17 | d[t] << (32-17)) ^ (d[t] >> 19 | d[t] << (32-19)) ^ (d[t] >> 10);
		uint32_t ausgabe = a[t] + s0 + c[t] + s1;
        uint32_t result = 0;

        Const_32 ca(a[t]);
        ca.setOutput(0);
        ca.append(&solver);

        Const_32 cb(b[t]);
        cb.setOutput(32);
        cb.append(&solver);

        Const_32 cc(c[t]);
        cc.setOutput(64);
        cc.append(&solver);

        Const_32 cd(d[t]);
        cd.setOutput(96);
        cd.append(&solver);

        Extension_32 extension;
        extension.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "EXTENSION UNSAT");

        unsigned output = extension.getOutput();
        for (unsigned i = output + 31; i >= output; i--) {
            result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - output));
        }

        mu_assert(ausgabe == result, "EXTENSION failed");
    }
}
