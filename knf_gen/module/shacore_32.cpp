#include "shacore_32.h"

#include "add_32.h"
#include "add_b0maj_32.h"
#include "add_b1ch_32.h"
#include "sub_32.h"

#include "../common/solvertools.h"

#define ADDITIONAL_KNOWLEDGE 0

using std::vector;
using namespace CMSat;

unsigned ShaCore_32::stats[STATS_LENGTH];

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

    Add_32 adder;
    Add_B0Maj_32 adderB0Maj;
    Add_B1Ch_32 adderB1Ch;
    output = start + adderB0Maj.getAdditionalVarCount() + adderB1Ch.getAdditionalVarCount() + 4 * adder.getAdditionalVarCount() - 64;
#if ADDITIONAL_KNOWLEDGE == 1
    Sub_32 subber;
    output += adder.getAdditionalVarCount() + subber.getAdditionalVarCount() - 32;
#endif
}

ShaCore_32::~ShaCore_32() {
}

unsigned* ShaCore_32::getStats() {
    return stats;
}

void ShaCore_32::create(Printer* printer) {
    printer->newModul(20, "ShaCore_32", this);

    unsigned newvars = 0;
    vector<unsigned> subinputs;

#if ADDITIONAL_KNOWLEDGE == 1
    Add_32 rev_adder;
    Sub_32 rev_subber;
    newvars = rev_adder.getAdditionalVarCount() + rev_subber.getAdditionalVarCount() - 32;
#endif

    subinputs.clear();
    subinputs.push_back(inputs[0]);
    subinputs.push_back(inputs[1]);
    subinputs.push_back(inputs[2]);
    Add_B0Maj_32 adderB0Maj;
    adderB0Maj.setInputs(subinputs);
    adderB0Maj.setStart(start + newvars);
    adderB0Maj.create(printer);
    newvars += adderB0Maj.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(inputs[4]);
    subinputs.push_back(inputs[5]);
    subinputs.push_back(inputs[6]);
    Add_B1Ch_32 adderB1Ch;
    adderB1Ch.setInputs(subinputs);
    adderB1Ch.setStart(start + newvars);
    adderB1Ch.create(printer);
    newvars += adderB1Ch.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(inputs[7]);
    subinputs.push_back(inputs[8]);
    Add_32 adder1;
    adder1.setInputs(subinputs);
    adder1.setStart(start + newvars);
    adder1.create(printer);
    newvars += adder1.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(adderB1Ch.getOutput());
    subinputs.push_back(adder1.getOutput());
    Add_32 adder2;
    adder2.setInputs(subinputs);
    adder2.setStart(start + newvars);
    adder2.create(printer);
    newvars += adder2.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(inputs[3]);
    subinputs.push_back(adder2.getOutput());
    Add_32 adder4;
    adder4.setInputs(subinputs);
    adder4.setStart(start + newvars);
    adder4.setOutput(adder4.getOutput() + adder4.getAdditionalVarCount());
    adder4.create(printer);
    newvars += adder4.getAdditionalVarCount() - 32;

    subinputs.clear();
    subinputs.push_back(adderB0Maj.getOutput());
    subinputs.push_back(adder2.getOutput());
    Add_32 adder3;
    adder3.setInputs(subinputs);
    adder3.setStart(start + newvars);
    adder3.create(printer);
    newvars += adder3.getAdditionalVarCount() + 32;

#ifdef ADDITIONAL_CLAUSES
    // XOR ->             731    289         321          637    
    createXOR(printer, output, start, start + 32, start + 348);
    // XOR ->                  637          416          448          574    
    createXOR(printer, start + 348, start + 127, start + 159, start + 285);
#endif

#if ADDITIONAL_KNOWLEDGE == 1
    newvars = 0;

    subinputs.clear();
    subinputs.push_back(adder4.getOutput());
    subinputs.push_back(adderB0Maj.getOutput());
    rev_adder.setInputs(subinputs);
    rev_adder.setStart(start + newvars);
    rev_adder.create(printer);
    newvars += rev_adder.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(rev_adder.getOutput());
    subinputs.push_back(adder3.getOutput());
    rev_subber.setInputs(subinputs);
    rev_subber.setStart(start + newvars);
    rev_subber.setOutput(inputs[3]);
    rev_subber.create(printer);
#endif
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

		uint32_t S0 = (a[t] >> 2 | a[t] << (32-2)) ^ (a[t] >> 13 | a[t] << (32-13)) ^ (a[t] >> 22 | a[t] << (32-22));
		uint32_t maj = (a[t] & b[t]) ^ (a[t] & c[t]) ^ (b[t] & c[t]);
		uint32_t t2 = S0 + maj;
		uint32_t S1 = (e[t] >> 6 | e[t] << (32-6)) ^ (e[t] >> 11 | e[t] << (32-11)) ^ (e[t] >> 25 | e[t] << (32-25));
		uint32_t fch = (e[t] & f[t]) ^ ((~e[t]) & g[t]);
		uint32_t t1 = h[t] + S1 + fch + i[t];
		uint32_t ausgabe[2] = {t1 + t2, d[t] + t1};

        solver_writeInt(solver,   0, 32, a[t]);
        solver_writeInt(solver,  32, 32, b[t]);
        solver_writeInt(solver,  64, 32, c[t]);
        solver_writeInt(solver,  96, 32, d[t]);
        solver_writeInt(solver, 128, 32, e[t]);
        solver_writeInt(solver, 160, 32, f[t]);
        solver_writeInt(solver, 192, 32, g[t]);
        solver_writeInt(solver, 224, 32, h[t]);
        solver_writeInt(solver, 256, 32, i[t]);

        ShaCore_32 shaCore;
        shaCore.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "SHACORE UNSAT");

        unsigned output = shaCore.getOutput();
        mu_assert(ausgabe[0] == solver_readInt(solver, output     , 32), "SHACORE 0 failed");
        mu_assert(ausgabe[1] == solver_readInt(solver, output + 32, 32), "SHACORE 1 failed");
    }
}
