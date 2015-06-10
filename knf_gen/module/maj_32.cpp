#include "maj_32.h"

#include "clausecreator.h"

// MAJ(x; y; z) = (a AND b) XOR (a AND c) XOR (b AND c)
// (!m | a | b) & (!m | a | c) & (!m | b | c) & (m | !a | !b) & (m | !a | !c) & (m | !b | !c)

// a =  0 -  31
// b = 32 -  63
// c = 64 -  95
// m = 96 - 127

Maj_32::Maj_32() : Modul(3, 32) {
    inputs.push_back(0);
    inputs.push_back(32);
    inputs.push_back(64);
    output = 96;
}

Maj_32::~Maj_32() {
}

void Maj_32::create(Printer* printer) {
    ClauseCreator cc(printer);
    for (unsigned i = 0; i < 32; i++) {
        //                     m_out           a_in           b_in           c_in
        cc.setLiterals(4, output + i, inputs[0] + i, inputs[1] + i, inputs[2] + i);
        cc.printClause(4,          0,             1,             1,         CC_DC);
        cc.printClause(4,          0,             1,         CC_DC,             1);
        cc.printClause(4,          0,         CC_DC,             1,             1);
        cc.printClause(4,          1,             0,             0,         CC_DC);
        cc.printClause(4,          1,             0,         CC_DC,             0);
        cc.printClause(4,          1,         CC_DC,             0,             0);
    }
}
