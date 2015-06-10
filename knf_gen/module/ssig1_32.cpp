#include "ssig1_32.h"

Ssig1_32::Ssig1_32() : Modul(1, 32) {
    inputs.push_back(0);
    output = 32;
}

Ssig1_32::~Ssig1_32() {
}

void Ssig1_32::create(Printer* printer) {
    for (unsigned i = 0; i < 22; i++) {
        createXOR(printer,
                  output + i, 
                  inputs[0] + ((i + 17) % 32),
                  inputs[0] + ((i + 19) % 32),
                  inputs[0] +   i + 10
        );
    }
    for (unsigned i = 22; i < 32; i++) {
        createXOR(printer,
                  output + i, 
                  inputs[0] + ((i + 17) % 32),
                  inputs[0] + ((i + 19) % 32)
        );
    }
}
