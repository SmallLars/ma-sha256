#include "shacore_ex1_32.h"

#include "constadd_32.h"
#include "shacore_32.h"
#include "clausecreator.h"

#include "../common/solvertools.h"

using std::vector;
using namespace CMSat;

unsigned ShaCore_Ex1_32::stats[STATS_LENGTH];

ShaCore_Ex1_32::ShaCore_Ex1_32(uint32_t value) : Modul(32, 9, 2) {
    this->value = value;

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
    printer->newModul(21, "ShaCore_Ex1_32", this);

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

#ifdef ADDITIONAL_CLAUSES
    ClauseCreator cc(printer);
    if ((value & 1) == 0) {
        // 1. inputbit    -    1. carrybit in 3rd adder of shacore
        cc.setLiterals(2, inputs[8], start + 317);
        cc.printClause(2,         1,           0);
    }
    // 1. carrybit of constadder    -    1. carrybit in 3rd adder of shacore
    cc.setLiterals(2, start, start + 317);
    cc.printClause(2,     0,           0);
#endif
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
        solver.set_verbosity(0);
        solver.log_to_file("test.log");

        uint32_t S0 = (a[t] >> 2 | a[t] << (32-2)) ^ (a[t] >> 13 | a[t] << (32-13)) ^ (a[t] >> 22 | a[t] << (32-22));
        uint32_t maj = (a[t] & b[t]) ^ (a[t] & c[t]) ^ (b[t] & c[t]);
        uint32_t t2 = S0 + maj;
        uint32_t S1 = (e[t] >> 6 | e[t] << (32-6)) ^ (e[t] >> 11 | e[t] << (32-11)) ^ (e[t] >> 25 | e[t] << (32-25));
        uint32_t fch = (e[t] & f[t]) ^ ((~e[t]) & g[t]);
        uint32_t t1 = h[t] + S1 + fch + input[t] + cons[t];
        uint32_t ausgabe[2] = {t1 + t2, d[t] + t1};

        solver_writeInt(solver,   0, 32, a[t]);
        solver_writeInt(solver,  32, 32, b[t]);
        solver_writeInt(solver,  64, 32, c[t]);
        solver_writeInt(solver,  96, 32, d[t]);
        solver_writeInt(solver, 128, 32, e[t]);
        solver_writeInt(solver, 160, 32, f[t]);
        solver_writeInt(solver, 192, 32, g[t]);
        solver_writeInt(solver, 224, 32, h[t]);
        solver_writeInt(solver, 256, 32, input[t]);

        ShaCore_Ex1_32 shaCore(cons[t]);
        shaCore.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "SHACORE UNSAT");

        unsigned output = shaCore.getOutput();
        mu_assert(ausgabe[0] == solver_readInt(solver, output     , 32), "SHACORE 0 failed");
        mu_assert(ausgabe[1] == solver_readInt(solver, output + 32, 32), "SHACORE 1 failed");
    }
}
