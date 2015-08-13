#include "sha256.h"

#include "const.h"
#include "add_prepare_32.h"
#include "shacore_ex1_32.h"
#include "clausecreator.h"
#include "../common/sha256tools.h"
#include "../common/solvertools.h"

using std::vector;
using namespace CMSat;

unsigned Sha256::stats[STATS_LENGTH];

static uint32_t sha_k[64] = {\
     0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,\
     0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,\
     0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,\
     0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,\
     0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,\
     0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,\
     0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,\
     0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2};

Sha256::Sha256() : Modul(32, 24, 8) {
    for (unsigned i = 0; i < 24; i++) {
        inputs.push_back(i * 32);
    }

    start = 768;
    output = 0;
}

Sha256::~Sha256() {
}

unsigned* Sha256::getStats() {
    return stats;
}

void Sha256::create(Printer* printer) {
    // Input
    unsigned global_input[64];
    for (unsigned i = 0; i < 16; i++) global_input[i] = inputs[i];

    // Status
    unsigned vars[8];
    for (unsigned i = 16; i < 24; i++) vars[i - 16] = inputs[i];

    // 512 Inputbits und 256 Statebits
    unsigned newvars = 0;

    Add_Prepare_32 adder;
    ShaCore_Ex1_32 core(0);

    for (unsigned i = 0; i < 64; i++) {
        vector<unsigned> subinputs;
        for (unsigned n = 0; n < 8; n++) subinputs.push_back(vars[n]);

        if (i < 16) {
            subinputs.push_back(global_input[i]);
        } else {
            vector<unsigned> prepareinputs;
            prepareinputs.push_back(global_input[i - 16]);
            prepareinputs.push_back(global_input[i - 15]);
            prepareinputs.push_back(global_input[i -  7]);
            prepareinputs.push_back(global_input[i -  2]);
            adder.setInputs(prepareinputs);
            adder.setStart(start + newvars);
            adder.create(printer);
            newvars += adder.getAdditionalVarCount();

            global_input[i] = adder.getOutput();
            subinputs.push_back(adder.getOutput());
        }

        core.setValue(sha_k[i]);
        core.setInputs(subinputs);
        core.setStart(start + newvars);
        core.create(printer);
        newvars += core.getAdditionalVarCount();

        for (unsigned n = 7; n > 0; n--) vars[n] = vars[n - 1];
        vars[0] = core.getOutput();
        vars[4] = core.getOutput() + 32;
    }
}

MU_TEST_C(Sha256::test) {
    SATSolver solver;
    solver.log_to_file("test.log");
//    solver.set_num_threads(4);

    Const con(32, 0);

    uint32_t input[16];
    sha256_padding(input, "Das ist eine Eingabe aus der ein Hash erstellt wird.");
    for (unsigned i = 0; i < 16; i++) {
        con.setValue(input[i]);
        con.setOutput(i * 32);
        con.append(&solver);
    }

    uint32_t state[8] = {0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19};
    for (unsigned i = 16; i < 24; i++) {
        con.setValue(state[i - 16]);
        con.setOutput(i * 32);
        con.append(&solver);
    }

    Sha256 sha256;
    sha256.append(&solver);

    lbool ret = solver.solve();
    mu_assert(ret == l_True, "SHA256 UNSAT");

    uint32_t output[8] = {0x27931f0e, 0x7e53670d, 0xdbec1a1c, 0xe23e21b4, 0x663c63c0, 0xd17117ee, 0x1a934bc0, 0xc294dbe9};
    uint32_t out_lsb[8] = {49264, 48442, 47620, 46798, 49296, 48474, 47652, 46830};

    for (unsigned i = 0; i < 8; i++) {
        mu_assert(output[i] - state[i] == solver_readInt(solver, out_lsb[i], 32), "SHA256 failed");
    }
}
