#include "sha256tools.h"

void sha256_padding(uint32_t target[16], const char* input) {
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
