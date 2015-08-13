#include "shacore_ex1_32.h"

#include "const.h"
#include "constadd_32.h"
#include "shacore_32.h"
#include "clausecreator.h"

using std::vector;
using namespace CMSat;

unsigned ShaCore_Ex1_32::stats[STATS_LENGTH];

ShaCore_Ex1_32::ShaCore_Ex1_32(uint32_t value) : Modul(32, 9, 2) {
    this->value = value;

    inputs.push_back(0);   // a
    inputs.push_back(32);  // b
    inputs.push_back(64);  // c
    inputs.push_back(96);  // d
    inputs.push_back(128); // e
    inputs.push_back(160); // f
    inputs.push_back(192); // g
    inputs.push_back(224); // h
    inputs.push_back(256); // Input
    
    start = 288;

    ConstAdd_32 adder(0);
    ShaCore_32 shacore;

    output = start + adder.getAdditionalVarCount() + shacore.getAdditionalVarCount() - 64;
}

ShaCore_Ex1_32::~ShaCore_Ex1_32() {
}

void ShaCore_Ex1_32::setValue(uint32_t value) {
    this->value = value;
}

unsigned* ShaCore_Ex1_32::getStats() {
    return stats;
}

void ShaCore_Ex1_32::create(Printer* printer) {
    unsigned newvars = 0;
    vector<unsigned> subinputs;

    subinputs.clear();
    subinputs.push_back(inputs[8]);
    ConstAdd_32 adder(value);
    adder.setInputs(subinputs);
    adder.setStart(start + newvars);
    adder.create(printer);
    newvars += adder.getAdditionalVarCount();

    subinputs = inputs;
    subinputs[8] = adder.getOutput();
    ShaCore_32 shacore;
    shacore.setInputs(subinputs);
    shacore.setStart(start + newvars);
    shacore.create(printer);
    newvars += shacore.getAdditionalVarCount();

    ClauseCreator cc(printer);
    if ((value & 1) == 0) {
        // 1. inputbit    -    1. carrybit in 3rd adder of shacore
        cc.setLiterals(2, inputs[8], start + 317);
        cc.printClause(2,         1,           0);
    }
    // 1. carrybit of constadder    -    1. carrybit in 3rd adder of shacore
    cc.setLiterals(2, start, start + 317);
    cc.printClause(2,     0,           0);
}

MU_TEST_C(ShaCore_Ex1_32::test) {
    unsigned a[] = {0xabcdef98};
    unsigned b[] = {0x651d8fa1};
    unsigned c[] = {0x456af012};
    unsigned d[] = {0x007fa4c5};
    unsigned e[] = {0xabcdef98};
    unsigned f[] = {0x651d8fa1};
    unsigned g[] = {0x456af012};
    unsigned h[] = {0x007fa4c5};
    unsigned input[] = {0xabcdef98};
    unsigned cons[] = {0xfa509e8c};

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");
        solver.set_num_threads(4);

		uint32_t S0 = (a[t] >> 2 | a[t] << (32-2)) ^ (a[t] >> 13 | a[t] << (32-13)) ^ (a[t] >> 22 | a[t] << (32-22));
		uint32_t maj = (a[t] & b[t]) ^ (a[t] & c[t]) ^ (b[t] & c[t]);
		uint32_t t2 = S0 + maj;
		uint32_t S1 = (e[t] >> 6 | e[t] << (32-6)) ^ (e[t] >> 11 | e[t] << (32-11)) ^ (e[t] >> 25 | e[t] << (32-25));
		uint32_t fch = (e[t] & f[t]) ^ ((~e[t]) & g[t]);
		uint32_t t1 = h[t] + S1 + fch + input[t] + cons[t];
		uint32_t ausgabe[2] = {t1 + t2, d[t] + t1};
        uint32_t result[2] = {0, 0};

        Const con(32, a[t]);
        con.setOutput(0);
        con.append(&solver);

        con.setValue(b[t]);
        con.setOutput(32);
        con.append(&solver);

        con.setValue(c[t]);
        con.setOutput(64);
        con.append(&solver);

        con.setValue(d[t]);
        con.setOutput(96);
        con.append(&solver);

        con.setValue(e[t]);
        con.setOutput(128);
        con.append(&solver);

        con.setValue(f[t]);
        con.setOutput(160);
        con.append(&solver);

        con.setValue(g[t]);
        con.setOutput(192);
        con.append(&solver);

        con.setValue(h[t]);
        con.setOutput(224);
        con.append(&solver);

        con.setValue(input[t]);
        con.setOutput(256);
        con.append(&solver);

        ShaCore_Ex1_32 shaCore(cons[t]);
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
