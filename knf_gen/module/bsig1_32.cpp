#include "bsig1_32.h"

Bsig1_32::Bsig1_32() : Modul(1, 32) {
    inputs.push_back(0);
    output = 32;
}

Bsig1_32::~Bsig1_32() {
}

void Bsig1_32::create(Printer* printer) {
    for (unsigned i = 0; i < 32; i++) {
        createXOR(printer,
                  output + i, 
                  inputs[0] + ((i +  6) % 32),
                  inputs[0] + ((i + 11) % 32),
                  inputs[0] + ((i + 25) % 32)
        );
    }
}
