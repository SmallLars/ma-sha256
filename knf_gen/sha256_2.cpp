#include <vector>
#include <stdio.h>

#include "cryptominisat4/cryptominisat.h"
#include "module/const.h"
#include "module/add_prepare_32.h"
#include "module/shacore_ex1_32.h"

using std::cout;
using std::vector;
using namespace CMSat;

static uint32_t sha_k[64] = {\
     0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,\
     0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,\
     0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,\
     0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,\
     0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,\
     0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,\
     0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,\
     0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2};

void padding(uint32_t* target, const char* input) {
    unsigned i;
    for (i = 0; i < 16; i++) target[i] = 0;

    char* t = (char*) target;
    for (i = 0; input[i] != '\0' ; i++) {
        t[(i / 4) * 4 + 3 - (i % 4)] = input[i];
    }
    t[(i / 4) * 4 + 3 - (i % 4)] = 0x80;
    target[15] = i * 8;

    //for (i = 0; i < 16; i++) printf("%08x ", target[i]);
}

int main() {
/*
    uint32_t input[16] = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                          0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x80000000, 0x00000000, 0x000001A0};

    uint32_t input[16] = {0x61626364, 0x65800000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                          0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000028};
*/
    uint32_t input[16];
    padding(input, "abcde");
    // 36bbe50e d96841d1 0443bcb6 70d6554f 0a34b761 be67ec9c 4a8ad2c0 c44ca42c

    uint32_t state[8] = {0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19};

    unsigned varCount = 0;

    SolverConf config;
    config.verbosity = 2;
    config.printFullStats = 1;
    config.doSQL = false;

    SATSolver solver(config);
//    solver.log_to_file("solver.log");
    solver.set_num_threads(4);

    for (unsigned i = 0; i < 16; i++) {
//        if (i != 11) {
            Const c(32, input[i]);
            c.setStart(i * 32);
            c.append(&solver);
            varCount += c.getAdditionalVarCount();
/*        } else {
            Const c(16, input[i]);
            c.setStart(i * 32);
            c.append(&solver);
            varCount += 32;
        }*/
    }

    cout << "1 / 3: Eingabe gesetzt.\n";

    unsigned vars[8];
    for (unsigned i = 0; i < 8; i++) {
        Const c(32, state[i]);
        c.setStart(varCount);
        c.append(&solver);
        varCount += c.getAdditionalVarCount();

        vars[i] = c.getOutput();
    }

    cout << "2 / 3: Status gesetzt.\n";

    Add_Prepare_32 adder;
    ShaCore_Ex1_32 core(0);

    unsigned global_input[64];
    for (unsigned i = 0; i < 16; i++) global_input[i] = i * 32;

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
            adder.setStart(varCount);
            adder.append(&solver);
            varCount += adder.getAdditionalVarCount();

            global_input[i] = adder.getOutput();
            subinputs.push_back(adder.getOutput());
        }

        core.setValue(sha_k[i]);
        core.setInputs(subinputs);
        core.setStart(varCount);
        core.append(&solver);
        varCount += core.getAdditionalVarCount();

        for (unsigned n = 7; n > 0; n--) vars[n] = vars[n - 1];
        vars[0] = core.getOutput();
        vars[4] = core.getOutput() + 32;

        cout << "\r3 / 3: Kern " << i + 1 << " / 64 definiert." << std::flush;
    }
    cout << "\n";

/*
    // START - Erste 32 Bit vom Ergebnis auf 0 setzen
    Const c1(32, 0x95F61999);
    c1.setStart(vars[0]);
    c1.append(&solver);
    // ENDE

    // START - Zweite 32 Bit vom Ergebnis auf 0 setzen
    Const c2(32, 0x4498517B);
    c2.setStart(vars[1]);
    c2.append(&solver);
    // ENDE
*/

    lbool ret = solver.solve();
    if (ret == l_False) {
        cout << "Nicht lösbar.\n";
        return 0;
    }

    cout << "Lösung gefunden\n";

    cout << "Eingabe: ";
    for (unsigned i = 0; i < 16; i++) {
        if (i == 8) cout << "\n         ";
        uint32_t result = 0;
        for (unsigned b = i * 32; b < (i + 1) * 32; b++) {
            result |= ((solver.get_model()[b] == l_True? 1 : 0) << (b - i * 32));
        }
        printf("%08x ", result);
    }

    cout << "\nAusgabe: ";
    for (unsigned i = 0; i < 8; i++) {
        uint32_t result = 0;
        for (unsigned b = vars[i]; b < vars[i] + 32; b++) {
            result |= ((solver.get_model()[b] == l_True? 1 : 0) << (b - vars[i]));
        }
        state[i] += result;

        printf("%08x ", state[i]);
    }
    cout << "\n";

    return 0;
}
