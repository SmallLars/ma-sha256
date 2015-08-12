#include <vector>
#include <stdio.h>
#include <iomanip>
#include <signal.h>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>

#include "module/const.h"
#include "module/add_prepare_32.h"
#include "module/shacore_ex1_32.h"

#include "printer/counter.h"
#include "printer/logger.h"
#include "printer/dimacsfileprinter.h"

using std::cout;
using std::vector;
using std::setw;
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
*/
    uint32_t input[16];
    padding(input, "Das ist eine Eingabe aus der ein Hash erstellt wird.");
    // 27931f0e 7e53670d dbec1a1c e23e21b4 663c63c0 d17117ee 1a934bc0 c294dbe9

    uint32_t state[8] = {0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19};

    unsigned varCount = 0;

    Counter counter;
    Logger logger("sha256.txt");
    DimacsFilePrinter printer("sha256.dimacs", 49328, 149454);

    for (unsigned i = 13; i < 16; i++) {
        Const c(32, input[i]);
        c.setStart(i * 32);
        c.create(&counter);
        c.create(&logger);
        c.create(&printer);
    }
    varCount += 512;

    cout << "  1 /   3: Eingabe gesetzt.\n";

    unsigned vars[8];
    for (unsigned i = 0; i < 8; i++) {
        Const c(32, state[i]);
        c.setStart(varCount);
        c.create(&counter);
        c.create(&logger);
        c.create(&printer);
        varCount += c.getAdditionalVarCount();

        vars[i] = c.getOutput();
    }

    cout << "  2 /   3: Status gesetzt.\n";

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
            adder.create(&counter);
            adder.create(&logger);
            adder.create(&printer);
            varCount += adder.getAdditionalVarCount();

            global_input[i] = adder.getOutput();
            subinputs.push_back(adder.getOutput());
        }

        core.setValue(sha_k[i]);
        core.setInputs(subinputs);
        core.setStart(varCount);
        core.create(&counter);
        core.create(&logger);
        core.create(&printer);
        varCount += core.getAdditionalVarCount();

        for (unsigned n = 7; n > 0; n--) vars[n] = vars[n - 1];
        vars[0] = core.getOutput();
        vars[4] = core.getOutput() + 32;

        cout << "\r  3 /   3: Kern " << i + 1 << " / " << 64 << " definiert." << std::flush;
    }
    cout << "\n";

    cout << "Variablen: " << counter.getMaxVar() << " Klauseln: " << counter.getClauseCount() << "\n";

    return 0;
}
