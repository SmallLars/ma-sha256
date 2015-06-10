#include "ch_32.h"

#include "clausecreator.h"

// CH(a; b; c) = (a AND b) XOR ( (NOT a) AND c)
// (!r | !a | b) & (!r | a | c) & (r |!a | !b) & (r | a | !c)

// a =  0 -  31
// b = 32 -  63
// c = 64 -  95
// r = 96 - 127

Ch_32::Ch_32() : Modul(3, 32) {
    inputs.push_back(0);
    inputs.push_back(32);
    inputs.push_back(64);
    output = 96;
}

Ch_32::~Ch_32() {
}

void Ch_32::create(Printer* printer) {
    ClauseCreator cc(printer);
    for (unsigned i = 0; i < 32; i++) {
        //                     r_out           a_in           b_in           c_in
        cc.setLiterals(4, output + i, inputs[0] + i, inputs[1] + i, inputs[2] + i);
        cc.printClause(4,          0,             0,             1,         CC_DC);
        cc.printClause(4,          0,             1,         CC_DC,             1);
        cc.printClause(4,          1,             0,             0,         CC_DC);
        cc.printClause(4,          1,             1,         CC_DC,             0);
    }
}
