#include "shacore_32.h"

#include "const_32.h"
#include "adder_32.h"
#include "adder_b0maj_32.h"
#include "adder_b1ch_32.h"

using std::vector;
using namespace CMSat;

ShaCore_32::ShaCore_32() : Modul(32, 9, 2) {
    inputs.push_back(0);
    inputs.push_back(32);
    inputs.push_back(64);
    inputs.push_back(96);
    inputs.push_back(128);
    inputs.push_back(160);
    inputs.push_back(192);
    inputs.push_back(224);
    inputs.push_back(256);
    
    start = 288;

    Adder_32 adder;
    Adder_B0Maj_32 adderB0Maj;
    Adder_B1Ch_32 adderB1Ch;
    output = start + adderB0Maj.getAdditionalVarCount() + adderB1Ch.getAdditionalVarCount() + 4 * adder.getAdditionalVarCount() - 64;
}

ShaCore_32::~ShaCore_32() {
}

void ShaCore_32::create(Printer* printer) {
    unsigned newvars = 0;
    vector<unsigned> subinputs;

    subinputs.clear();
    subinputs.push_back(inputs[0]);
    subinputs.push_back(inputs[1]);
    subinputs.push_back(inputs[2]);
    Adder_B0Maj_32 adderB0Maj;
    adderB0Maj.setInputs(subinputs);
    adderB0Maj.setStart(start + newvars);
    adderB0Maj.create(printer);
    newvars += adderB0Maj.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(inputs[4]);
    subinputs.push_back(inputs[5]);
    subinputs.push_back(inputs[6]);
    Adder_B1Ch_32 adderB1Ch;
    adderB1Ch.setInputs(subinputs);
    adderB1Ch.setStart(start + newvars);
    adderB1Ch.create(printer);
    newvars += adderB1Ch.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(inputs[7]);
    subinputs.push_back(inputs[8]);
    Adder_32 adder1;
    adder1.setInputs(subinputs);
    adder1.setStart(start + newvars);
    adder1.create(printer);
    newvars += adder1.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(adderB1Ch.getOutput());
    subinputs.push_back(adder1.getOutput());
    Adder_32 adder2;
    adder2.setInputs(subinputs);
    adder2.setStart(start + newvars);
    adder2.create(printer);
    newvars += adder2.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(inputs[3]);
    subinputs.push_back(adder2.getOutput());
    Adder_32 adder4;
    adder4.setInputs(subinputs);
    adder4.setStart(start + newvars);
    adder4.setOutput(adder4.getOutput() + adder4.getAdditionalVarCount());
    adder4.create(printer);
    newvars += adder4.getAdditionalVarCount() - 32;

    subinputs.clear();
    subinputs.push_back(adderB0Maj.getOutput());
    subinputs.push_back(adder2.getOutput());
    Adder_32 adder3;
    adder3.setInputs(subinputs);
    adder3.setStart(start + newvars);
    adder3.create(printer);
    newvars += adder3.getAdditionalVarCount() + 32;
}

MU_TEST_C(ShaCore_32::test) {
    unsigned a[] = {0xabcdef98};
    unsigned b[] = {0x651d8fa1};
    unsigned c[] = {0x456af012};
    unsigned d[] = {0x007fa4c5};
    unsigned e[] = {0xabcdef98};
    unsigned f[] = {0x651d8fa1};
    unsigned g[] = {0x456af012};
    unsigned h[] = {0x007fa4c5};
    unsigned i[] = {0xabcdef98};

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");
        solver.set_num_threads(4);

		uint32_t S0 = (a[t] >> 2 | a[t] << (32-2)) ^ (a[t] >> 13 | a[t] << (32-13)) ^ (a[t] >> 22 | a[t] << (32-22));
		uint32_t maj = (a[t] & b[t]) ^ (a[t] & c[t]) ^ (b[t] & c[t]);
		uint32_t t2 = S0 + maj;
		uint32_t S1 = (e[t] >> 6 | e[t] << (32-6)) ^ (e[t] >> 11 | e[t] << (32-11)) ^ (e[t] >> 25 | e[t] << (32-25));
		uint32_t fch = (e[t] & f[t]) ^ ((~e[t]) & g[t]);
		uint32_t t1 = h[t] + S1 + fch + i[t];
		uint32_t ausgabe[2] = {t1 + t2, d[t] + t1};
        uint32_t result[2] = {0, 0};

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

        Const_32 ce(e[t]);
        ce.setOutput(128);
        ce.append(&solver);

        Const_32 cf(f[t]);
        cf.setOutput(160);
        cf.append(&solver);

        Const_32 cg(g[t]);
        cg.setOutput(192);
        cg.append(&solver);

        Const_32 ch(h[t]);
        ch.setOutput(224);
        ch.append(&solver);

        Const_32 ci(i[t]);
        ci.setOutput(256);
        ci.append(&solver);

        ShaCore_32 shaCore;
        shaCore.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "SHACORE UNSAT");

        unsigned output = shaCore.getOutput();
        for (unsigned i = output + 31; i >= output; i--) {
            result[0] |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - output));
        }
        for (unsigned i = output + 63; i >= output + 32; i--) {
            result[1] |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - output));
        }

        mu_assert(ausgabe[0] == result[0], "SHACORE 0 failed");
        mu_assert(ausgabe[1] == result[1], "SHACORE 1 failed");
    }
}
