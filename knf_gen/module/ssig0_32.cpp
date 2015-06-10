#include "ssig0_32.h"

Ssig0_32::Ssig0_32() : Modul(1, 32) {
    inputs.push_back(0);
    output = 32;
}

Ssig0_32::~Ssig0_32() {
}

void Ssig0_32::create(Printer* printer) {
    for (unsigned i = 0; i < 29; i++) {
        createXOR(printer,
                  output + i, 
                  inputs[0] + ((i +  7) % 32),
                  inputs[0] + ((i + 18) % 32),
                  inputs[0] +   i +  3
        );
    }
    for (unsigned i = 29; i < 32; i++) {
        createXOR(printer,
                  output + i, 
                  inputs[0] + ((i +  7) % 32),
                  inputs[0] + ((i + 18) % 32)
        );
    }
}
