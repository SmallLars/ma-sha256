#include "shacore_ex2_32.h"

#include "add_prepare_32.h"
#include "shacore_ex1_32.h"

#include "../common/solvertools.h"

using std::vector;
using namespace CMSat;

unsigned ShaCore_Ex2_32::stats[STATS_LENGTH];

ShaCore_Ex2_32::ShaCore_Ex2_32(uint32_t value) : Modul(32, 12, 2) {
    this->value = value;

    inputs.push_back(0);   // a
    inputs.push_back(32);  // b
    inputs.push_back(64);  // c
    inputs.push_back(96);  // d
    inputs.push_back(128); // e
    inputs.push_back(160); // f
    inputs.push_back(192); // g
    inputs.push_back(224); // h
    inputs.push_back(256); // Input 1
    inputs.push_back(288); // Input 2
    inputs.push_back(320); // Input 3
    inputs.push_back(352); // Input 4
    
    start = 384;

    Add_Prepare_32 adder;
    ShaCore_Ex1_32 shacore(0);

    output = start + adder.getAdditionalVarCount() + shacore.getAdditionalVarCount() - 64;
}

ShaCore_Ex2_32::~ShaCore_Ex2_32() {
}

void ShaCore_Ex2_32::setValue(uint32_t value) {
    this->value = value;
}

unsigned* ShaCore_Ex2_32::getStats() {
    return stats;
}

void ShaCore_Ex2_32::create(Printer* printer) {
    unsigned newvars = 0;
    vector<unsigned> subinputs;

    subinputs.clear();
    subinputs.push_back(inputs[8]);
    subinputs.push_back(inputs[9]);
    subinputs.push_back(inputs[10]);
    subinputs.push_back(inputs[11]);
    Add_Prepare_32 adder;
    adder.setInputs(subinputs);
    adder.setStart(start + newvars);
    adder.create(printer);
    newvars += adder.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(inputs[0]);
    subinputs.push_back(inputs[1]);
    subinputs.push_back(inputs[2]);
    subinputs.push_back(inputs[3]);
    subinputs.push_back(inputs[4]);
    subinputs.push_back(inputs[5]);
    subinputs.push_back(inputs[6]);
    subinputs.push_back(inputs[7]);
    subinputs.push_back(adder.getOutput());
    ShaCore_Ex1_32 shacore(value);
    shacore.setInputs(subinputs);
    shacore.setStart(start + newvars);
    shacore.create(printer);
    newvars += shacore.getAdditionalVarCount();
}

MU_TEST_C(ShaCore_Ex2_32::test) {
    unsigned a[] = {0xabcdef98};
    unsigned b[] = {0x651d8fa1};
    unsigned c[] = {0x456af012};
    unsigned d[] = {0x007fa4c5};
    unsigned e[] = {0xabcdef98};
    unsigned f[] = {0x651d8fa1};
    unsigned g[] = {0x456af012};
    unsigned h[] = {0x007fa4c5};
    unsigned input1[] = {0xfa509e8c};
    unsigned input2[] = {0xabcdef98};
    unsigned input3[] = {0xfa509e8c};
    unsigned input4[] = {0xabcdef98};
    unsigned cons[] = {0xfa509e8c};

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");

		uint32_t p0 = (input2[t] >> 7 | input2[t] << (32-7)) ^ (input2[t] >> 18 | input2[t] << (32-18)) ^ (input2[t] >> 3);
		uint32_t p1 = (input4[t] >> 17 | input4[t] << (32-17)) ^ (input4[t] >> 19 | input4[t] << (32-19)) ^ (input4[t] >> 10);
		uint32_t S0 = (a[t] >> 2 | a[t] << (32-2)) ^ (a[t] >> 13 | a[t] << (32-13)) ^ (a[t] >> 22 | a[t] << (32-22));
		uint32_t maj = (a[t] & b[t]) ^ (a[t] & c[t]) ^ (b[t] & c[t]);
		uint32_t t2 = S0 + maj;
		uint32_t S1 = (e[t] >> 6 | e[t] << (32-6)) ^ (e[t] >> 11 | e[t] << (32-11)) ^ (e[t] >> 25 | e[t] << (32-25));
		uint32_t fch = (e[t] & f[t]) ^ ((~e[t]) & g[t]);
		uint32_t t1 = h[t] + S1 + fch + cons[t] + input1[t] + p0 + input3[t] + p1;
		uint32_t ausgabe[2] = {t1 + t2, d[t] + t1};

        solver_writeInt(solver,   0, 32, a[t]);
        solver_writeInt(solver,  32, 32, b[t]);
        solver_writeInt(solver,  64, 32, c[t]);
        solver_writeInt(solver,  96, 32, d[t]);
        solver_writeInt(solver, 128, 32, e[t]);
        solver_writeInt(solver, 160, 32, f[t]);
        solver_writeInt(solver, 192, 32, g[t]);
        solver_writeInt(solver, 224, 32, h[t]);
        solver_writeInt(solver, 256, 32, input1[t]);
        solver_writeInt(solver, 288, 32, input2[t]);
        solver_writeInt(solver, 320, 32, input3[t]);
        solver_writeInt(solver, 352, 32, input4[t]);

        ShaCore_Ex2_32 shaCore(cons[t]);
        shaCore.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "SHACORE UNSAT");

        unsigned output = shaCore.getOutput();
        mu_assert(ausgabe[0] == solver_readInt(solver, output     , 32), "SHACORE 0 failed");
        mu_assert(ausgabe[1] == solver_readInt(solver, output + 32, 32), "SHACORE 1 failed");
    }
}
