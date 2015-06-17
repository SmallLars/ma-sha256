#include <vector>
#include <stdio.h>

#include "cryptominisat4/cryptominisat.h"
#include "module/const_32.h"
#include "module/prepare_32.h"
#include "module/shacore_32.h"

using std::cout;
using std::vector;
using namespace CMSat;

int main() {
    uint32_t input[16] = {0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                          0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x80000000, 0x00000000, 0x00000000};

    uint32_t state[8] = {0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19};

    unsigned varCount = 0;

    SATSolver solver;
    solver.log_to_file("solver.log");
    solver.set_num_threads(4);

    for (unsigned i = 0; i < 16; i++) {
        Const_32 c(input[i]);
        c.setStart(i * 32);
        c.append(&solver);
        varCount += c.getAdditionalVarCount();
    }

    cout << "1 / 4: Eingabe gesetzt.\n";

    Prepare_32 prepare;
    prepare.append(&solver);
    varCount += prepare.getAdditionalVarCount();
    uint32_t message = prepare.getOutput();

    cout << "2 / 4: Preparation definiert.\n";

    unsigned vars[8];
    for (unsigned i = 0; i < 8; i++) {
        Const_32 c(state[i]);
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

    lbool ret = solver.solve();
    if (ret == l_False) {
        cout << "Nicht lösbar.\n";
        return 0;
    }

    cout << "Lösung gefunden\n";
    // for (unsigned i = 0; i < 8; i++) cout << vars[i] << " ";
    // cout << "\n";

    for (unsigned i = 0; i < 8; i++) {
        uint32_t result = 0;
        for (unsigned b = vars[i] + 31; b >= vars[i]; b--) {
            result |= ((solver.get_model()[b] == l_True? 1 : 0) << (b - vars[i]));
        }
        state[i] += result;

        printf("%0x ", state[i]);
    }
    cout << "\n";

    // e3b0c442 98fc1c14 9afbf4c8 996fb924 27ae41e4 649b934c a495991b 7852b855

    return 0;
}
