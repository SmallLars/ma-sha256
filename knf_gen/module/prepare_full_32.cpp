#include "prepare_full_32.h"

#include "prepare_32.h"
#include "constadd_32.h"

#include "../common/solvertools.h"

using std::vector;
using namespace CMSat;

unsigned Prepare_Full_32::stats[STATS_LENGTH];

static uint32_t sha_k[64] = {\
     0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,\
     0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,\
     0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,\
     0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,\
     0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,\
     0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,\
     0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,\
     0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2};

Prepare_Full_32::Prepare_Full_32() : Modul(32, 16, 64) {
    Prepare_32 adderPrepare;
    ConstAdd_32 constAdder(0);
    output = start + 48 * adderPrepare.getAdditionalVarCount() + 64 * constAdder.getAdditionalVarCount() - 2048;
}

Prepare_Full_32::~Prepare_Full_32() {
}

unsigned* Prepare_Full_32::getStats() {
    return stats;
}

void Prepare_Full_32::create(Collector* collector) {
    collector->newModul(20, "Prepare_Full_32", this);

    unsigned newvars = 0;
    vector<unsigned> subinputs;
    vector<unsigned> results(inputs);
    results.resize(64);

    for (unsigned i = 16; i < 64; i++) {
        subinputs.clear();
        subinputs.push_back(results[i - 16]);
        subinputs.push_back(results[i - 15]);
        subinputs.push_back(results[i -  7]);
        subinputs.push_back(results[i -  2]);
        Prepare_32 adder;
        adder.setInputs(subinputs);
        adder.setStart(start + newvars);
        adder.create(collector);
        results[i] = adder.getOutput();
        newvars += adder.getAdditionalVarCount();
    }

    for (unsigned i = 0; i < 64; i++) {
        subinputs.clear();
        subinputs.push_back(results[i]);
        ConstAdd_32 adder(sha_k[i]);
        adder.setInputs(subinputs);
        adder.setStart(start + newvars);
        adder.setOutput(output + i * 32);
        adder.create(collector);
        newvars += adder.getAdditionalVarCount() - 32;
    }
}

MU_TEST_C(Prepare_Full_32::test) {
    unsigned values[16][1] = {
        {0xabcdef98},
        {0x651d8fa1},
        {0x456af012},
        {0x007fa4c5},
        {0xabcdef98},
        {0x651d8fa1},
        {0x456af012},
        {0x007fa4c5},
        {0xabcdef98},
        {0x651d8fa1},
        {0x456af012},
        {0x007fa4c5},
        {0xabcdef98},
        {0x651d8fa1},
        {0x456af012},
        {0x007fa4c5}
    };

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.set_verbosity(0);
        solver.log_to_file("test.log");

        uint32_t ausgabe[64];
        for (unsigned i = 0; i < 16; i++) ausgabe[i] = values[i][t];
        for (unsigned i = 16; i < 64; i++) {
            uint32_t s0 = (ausgabe[i-15] >> 7 | ausgabe[i-15] << (32-7)) ^ (ausgabe[i-15] >> 18 | ausgabe[i-15] << (32-18)) ^ (ausgabe[i-15] >> 3);
            uint32_t s1 = (ausgabe[i-2] >> 17 | ausgabe[i-2] << (32-17)) ^ (ausgabe[i-2] >> 19 | ausgabe[i-2] << (32-19)) ^ (ausgabe[i-2] >> 10);
            ausgabe[i] = ausgabe[i-16] + s0 + ausgabe[i-7] + s1;
        }
        for (unsigned i = 0; i < 64; i++) {
            ausgabe[i] += sha_k[i];
        }

        for (unsigned i = 0; i < 16; i++) {
            solver_writeInt(solver, i * 32, 32, values[i][t]);
        }

        Prepare_Full_32 prepare;
        prepare.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "PREPARE UNSAT");

        unsigned output = prepare.getOutput();
        for (unsigned i = 0; i < 64; i++) {
            mu_assert(ausgabe[i] == solver_readInt(solver, output, 32), "PREPARE failed");
            output += 32;
        }
    }
}
