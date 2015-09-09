#include "sha256.h"

#include "add_prepare_32.h"
#include "shacore_ex1_32.h"
#include "clausecreator.h"

#include "../common/sha256tools.h"
#include "../common/solvertools.h"
#include "../common/general.h"

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
    // Information for additional clauses
    unsigned prepNum[64];
    unsigned coreInputs[64][8];
    unsigned coreNum[64];

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
        for (unsigned n = 0; n < 8; n++) {
            subinputs.push_back(vars[n]);
            coreInputs[i][n] = vars[n];
        }

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
            prepNum[i] = start + newvars;
            adder.create(printer);
            newvars += adder.getAdditionalVarCount();

            global_input[i] = adder.getOutput();
            subinputs.push_back(adder.getOutput());
        }

        core.setValue(sha_k[i]);
        core.setInputs(subinputs);
        core.setStart(start + newvars);
        coreNum[i] = start + newvars;
        core.create(printer);
        newvars += core.getAdditionalVarCount();

        for (unsigned n = 7; n > 0; n--) vars[n] = vars[n - 1];
        vars[0] = core.getOutput();
        vars[4] = core.getOutput() + 32;
    }

#ifdef ADDITIONAL_CLAUSES
    ClauseCreator cc(printer);

    for (unsigned i = 0; i < 64; i++) {
        /*
        std::cout << (i < 16 ? 0 : prepNum[i] + 1);
        std::cout << " " << coreInputs[i][0] + 1 << " " << coreInputs[i][1] + 1 << " " << coreInputs[i][2] + 1 << " " << coreInputs[i][3] + 1;
        std::cout << " " << coreInputs[i][4] + 1 << " " << coreInputs[i][5] + 1 << " " << coreInputs[i][6] + 1 << " " << coreInputs[i][7] + 1;
        std::cout << " " << coreNum[i] + 1 << "\n";
        */

        // distance - modulcount + 1 = 4
        if (i < 39) {
            if (!in_array(i, 8, 2, 9, 11, 13, 20, 30, 36, 38)) {
//              -737 1088 -10002 -10033 -11772 17400 0
                cc.setLiterals(6, coreInputs[i][7], coreNum[i] + 319, prepNum[i + 16] + 129, prepNum[i + 16] + 160, coreNum[i + 18] + 2, prepNum[i + 25] + 129);
                cc.printClause(6,                0,                1,                     0,                     0,                   0,                     1);
            }
        }
        if (i < 48) {
            if (in_array(i, 9, 0, 3, 4, 14, 18, 25, 29, 40, 42)) {
//              -641 -642 2508 -7915 7977 -7978 12498 0
                cc.setLiterals(7, coreInputs[i][7], coreInputs[i][7] + 1, coreNum[i] + 32, coreNum[i + 9] + 318,
                                  coreNum[i + 9] + 380, coreNum[i + 9] + 381, prepNum[i + 16] + 159);
                cc.printClause(7,                0,                    0,               1,                    0,
                                                     1,                    0,                     1);
            }
        }

        // distance - modulcount + 1 = 3
        if (i < 39) {
            if (in_array(i, 11, 0, 6, 8, 12, 15, 19, 22, 26, 33, 34, 36)) {
//              738 -1087 10001 -17399 0
                cc.setLiterals(4, coreInputs[i][7] + 1, coreNum[i] + 318, prepNum[i + 16] + 128, prepNum[i + 25] + 128);
                cc.printClause(4,                    1,                0,                     1,                     0);
            }
            if (in_array(i, 3, 0, 8, 23)) {
//              773 -10035 -11775 17403 0
                cc.setLiterals(4, coreNum[i] + 4, prepNum[i + 16] + 162, coreNum[i + 18] + 5, prepNum[i + 25] + 132);
                cc.printClause(4,              1,                     0,                   0,                     1);
            }
        }
        if (i < 48) {
            if (in_array(i, 17, 0, 3, 6, 8, 12, 15, 19, 22, 25, 26, 33, 34, 36, 40, 42, 45, 47)) {
//              -2477 -7915 12467 0
                cc.setLiterals(3, coreNum[i] + 1, coreNum[i + 9] + 318, prepNum[i + 16] + 128);
                cc.printClause(3,              0,                    0,                     1);
            }
            if (in_array(i, 15, 2, 4, 11, 14, 20, 21, 23, 27, 28, 29, 31, 35, 37, 41, 43)) {
//              15881 -23596 -28907 0
                cc.setLiterals(3, coreNum[i] + 1, coreNum[i + 9] + 318, prepNum[i + 16] + 128);
                cc.printClause(3,              1,                    0,                     0);
            }
            if (in_array(i, 6, 2, 10, 13, 18, 19, 34)) {
//              -680 2232 7036 -11652 0
                cc.setLiterals(4, coreInputs[i][7] + 7, coreNum[i] + 325, coreNum[i + 9] + 8, prepNum[i + 16] + 135);
                cc.printClause(4,                    0,                1,                  1,                     0);
            }
            if (!in_array(i, 17, 2, 5, 7, 11, 16, 18, 20, 21, 23, 28, 30, 31, 32, 35, 37, 43, 46)) {
//              738 -1087 -5890 10001 0
                cc.setLiterals(4, coreInputs[i][7] + 1, coreNum[i] + 318, coreNum[i + 9], prepNum[i + 16] + 128);
                cc.printClause(4,                    1,                0,          0,                     1);
            }
            if (in_array(i, 8, 0, 3, 6, 25, 33, 36, 40, 42)) {
//              738 -1087 -5922 10001 0
                cc.setLiterals(4, coreInputs[i][7] + 1, coreNum[i] + 318, coreNum[i + 9] + 32, prepNum[i + 16] + 128);
                cc.printClause(4,                    1,                0,               0,                     1);
            }
            if (in_array(i, 9, 0, 3, 4, 14, 18, 25, 29, 40, 42)) {
//              738 -1087 5922 10032 0
                cc.setLiterals(4, coreInputs[i][7] + 1, coreNum[i] + 318, coreNum[i + 9] + 32, prepNum[i + 16] + 159);
                cc.printClause(4,                    1,                0,               1,                     1);
//              738 -1087 -5922 -10032 0
                cc.setLiterals(4, coreInputs[i][7] + 1, coreNum[i] + 318, coreNum[i + 9] + 32, prepNum[i + 16] + 159);
                cc.printClause(4,                    1,                0,               0,                     0);
            }
        }
    }
/*
    for (unsigned i = 0; i < 48; i++) {
//      1338 -6777 -10823 10853 0
        cc.setLiterals(4, coreNum[i], coreNum[i + 9] + 318, prepNum[i + 16] + 128, prepNum[i + 16] + 158);
        cc.printClause(4,          1,                    0,                     0,                     1);
    }
    for (unsigned i = 0; i < 48; i++) {
//      -1339 4721 6808 10823 0
        cc.setLiterals(4, coreNum[i] + 1, coreInputs[i + 6][4] + 1, coreNum[i + 9] + 349, prepNum[i + 16] + 128);
        cc.printClause(4,              0,                        1,                    1,                     1);
    }
    for (unsigned i = 0; i < 39; i++) {
//      -1339 10824 12594 -18222 0
        cc.setLiterals(4, coreNum[i] + 1, prepNum[i + 16] + 129, coreNum[i + 18] + 2, prepNum[i + 25] + 129);
        cc.printClause(4,              0,                     1,                   1,                     0);
    }
    for (unsigned i = 0; i < 39; i++) {
//      -1339 10824 12595 -18223 0
        cc.setLiterals(4, coreNum[i] + 1, prepNum[i + 16] + 129, coreNum[i + 18] + 3, prepNum[i + 25] + 130);
        cc.printClause(4,              0,                     1,                   1,                     0);
    }
    for (unsigned i = 0; i < 48; i++) {
//      -2476 5859 -7915 12467 0
        cc.setLiterals(4, coreNum[i], coreInputs[i + 6][4] + 1, coreNum[i + 9] + 318, prepNum[i + 16] + 128);
        cc.printClause(4,          0,                        1,                    0,                     1);
    }
    for (unsigned i = 0; i < 48; i++) {
//      -2477 5859 -7946 12467 0
        cc.setLiterals(4, coreNum[i] + 1, coreInputs[i + 6][4] + 1, coreNum[i + 9] + 349, prepNum[i + 16] + 128);
        cc.printClause(4,              0,                        1,                    0,                     1);
    }
    for (unsigned i = 0; i < 48; i++) {
//      -2477 -7915 -12497 -12498 0
        cc.setLiterals(4, coreNum[i] + 1, coreNum[i + 9] + 318, prepNum[i + 16] + 158, prepNum[i + 16] + 159);
        cc.printClause(4,              0,                    0,                     0,                     0);
    }
    for (unsigned i = 0; i < 39; i++) {
//      2478 -12468 -14238 19897 0
        cc.setLiterals(4, coreNum[i] + 2, prepNum[i + 16] + 129, coreNum[i + 18] + 2, prepNum[i + 25] + 160);
        cc.printClause(4,              1,                     0,                   0,                     1);
    }
    for (unsigned i = 0; i < 39; i++) {
//      2500 -12490 -14260 19919 0
        cc.setLiterals(4, coreNum[i] + 24, prepNum[i + 16] + 151, coreNum[i + 18] + 24, prepNum[i + 25] + 182);
        cc.printClause(4,               1,                     0,                    0,                     1);
    }
    for (unsigned i = 0; i < 39; i++) {
//      -2508 12467 14237 -19865 0
        cc.setLiterals(4, coreNum[i] + 32, prepNum[i + 16] + 128, coreNum[i + 18] + 1, prepNum[i + 25] + 128);
        cc.printClause(4,               0,                     1,                   1,                     0);
    }
    for (unsigned i = 0; i < 45; i++) {
//      -3582 5639 10949 -16577 0
        cc.setLiterals(4, coreInputs[i][4], coreNum[i + 3] + 318, coreNum[i + 12] + 1, prepNum[i + 19] + 128);
        cc.printClause(4,                0,                    1,                   1,                     0);
    }
    for (unsigned i = 0; i < 39; i++) {
//      5333 -16588 -18358 23986 0
        cc.setLiterals(4, coreNum[i] + 12, prepNum[i + 16] + 139, coreNum[i + 18] + 12, prepNum[i + 25] + 139);
        cc.printClause(4,               1,                     0,                    0,                     1);
    }
    for (unsigned i = 0; i < 39; i++) {
//      8736 -21572 -23279 28907 0
        cc.setLiterals(4, coreNum[i] + 1, prepNum[i + 16] + 191, coreNum[i + 18] + 1, prepNum[i + 25] + 128);
        cc.printClause(4,              1,                     0,                   0,                     1);
    }
*/
#endif
}

MU_TEST_C(Sha256::test) {
    SolverConf config;
    config.verbosity = 0;//9;

    SATSolver solver(config);
    solver.log_to_file("test.log");

    uint32_t input[16];
    sha256_padding(input, "Das ist eine Eingabe aus der ein Hash erstellt wird.");
    for (unsigned i = 0; i < 16; i++) {
        solver_writeInt(solver, i * 32, 32, input[i]);
    }

    uint32_t state[8] = {0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19};
    for (unsigned i = 16; i < 24; i++) {
        solver_writeInt(solver, i * 32, 32, state[i - 16]);
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
