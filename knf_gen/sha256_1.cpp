#include <vector>
#include <stdio.h>

#include "cryptominisat4/cryptominisat.h"
#include "module/const.h"
#include "module/prepare_full_32.h"
#include "module/shacore_32.h"

using std::cout;
using std::vector;
using namespace CMSat;

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

    SATSolver solver;
    solver.set_verbosity(2);
    solver.set_num_threads(4);
//    solver.log_to_file("solver.log");

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

    cout << "1 / 4: Eingabe gesetzt.\n";

    Prepare_Full_32 prepare;
    prepare.append(&solver);
    varCount += prepare.getAdditionalVarCount();
    uint32_t message = prepare.getOutput();

    cout << "2 / 4: Preparation definiert.\n";

    unsigned vars[8];
    for (unsigned i = 0; i < 8; i++) {
        Const c(32, state[i]);
        c.setStart(varCount);
        c.append(&solver);
        varCount += c.getAdditionalVarCount();

        vars[i] = c.getOutput();
    }

    cout << "3 / 4: Status gesetzt.\n";

    for (unsigned i = 0; i < 64; i++) {
        vector<unsigned> subinputs;
        for (unsigned n = 0; n < 8; n++) subinputs.push_back(vars[n]);
        subinputs.push_back(message + i * 32);

        ShaCore_32 core;
        core.setInputs(subinputs);
        core.setStart(varCount);
        core.append(&solver);
        varCount += core.getAdditionalVarCount();

        for (unsigned n = 7; n > 0; n--) vars[n] = vars[n - 1];
        vars[0] = core.getOutput();
        vars[4] = core.getOutput() + 32;

        cout << "\r4 / 4: Kern " << i + 1 << " / 64 definiert." << std::flush;
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
