#include "sha256tools.h"

uint32_t sha_k[64] = {
    0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
    0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
    0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
    0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
    0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
    0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
    0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
    0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
};

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

void sha256_calc(uint32_t *state, uint32_t *input) {
    uint32_t w[64], s0, s1;
    uint32_t a,b,c,d,e,f,g,h;
    uint32_t t1, t2, maj, ch, S0, S1;
    int n;

    // Read in input. When these 32bit words were read, they should have been taken as big endian.
    for (n = 0; n < 16; n++)
        w[n] = *(input + n);

    // Extend the sixteen 32-bit words into sixty-four 32-bit words:
    for (n = 16; n < 64; n++) {
        s0 = (w[n-15] >> 7 | w[n-15] << (32-7)) ^ (w[n-15] >> 18 | w[n-15] << (32-18)) ^ (w[n-15] >> 3);
        s1 = (w[n-2] >> 17 | w[n-2] << (32-17)) ^ (w[n-2] >> 19 | w[n-2] << (32-19)) ^ (w[n-2] >> 10);
        w[n] = w[n-16] + s0 + w[n-7] + s1;
    }

    for (n = 0; n < 64; n++) {
        w[n] += sha_k[n];
    }

    // Initialize hash value for this input:
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    f = state[5];
    g = state[6];
    h = state[7];

    // Main loop:
    for (n = 0; n < 64; n++) {
        S0 = (a >> 2 | a << (32-2)) ^ (a >> 13 | a << (32-13)) ^ (a >> 22 | a << (32-22));
        maj = (a & b) ^ (a & c) ^ (b & c);
        t2 = S0 + maj;
        S1 = (e >> 6 | e << (32-6)) ^ (e >> 11 | e << (32-11)) ^ (e >> 25 | e << (32-25));
        ch = (e & f) ^ ((~e) & g);
        t1 = h + S1 + ch + w[n];

        h = g; g = f; f = e; e = d + t1;
        d = c; c = b; b = a; a = t1 + t2;
    }

    // Add this input's hash to result so far:
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}
