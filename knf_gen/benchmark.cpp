#include <ctime>
#include <stdio.h>
#include <cmath>

#include "common/sha256tools.h"

int main(int argc, const char* argv[]) {
    uint32_t input[16];
    sha256_padding(input, "Das ist eine Eingabe aus der ein Hash erstellt wird.");

    time_t start_time = time(0);
    uint32_t state[8] = {0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19};
    for (unsigned i = 0; true; i++) {
        sha256_calc(state, input);
        if (i % 100000 == 0) {
            double hs = (double) i / (time(0) - start_time);
            printf("%f Hashes pro Sekunde. ", hs);
            printf("%f Bit an einem Tag.\r", log2(hs * 86400));
        }
    }

    return 0;
}
