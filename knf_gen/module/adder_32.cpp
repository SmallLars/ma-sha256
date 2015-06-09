#include "adder_32.h"

using std::vector;
using namespace CMSat;

Adder_32::Adder_32() : Modul(2, 32) {
    inputs.push_back(0);
    inputs.push_back(32);
    start = 64;
    output = 95;
}

Adder_32::~Adder_32() {
}

void Adder_32::create(Printer* printer) {
    vector<Lit> clause;
    // Half adder
#ifdef XOR_SUPPORT
        // (u | !a | !b)
        clause.clear();
        clause.push_back(Lit(start, false));
        clause.push_back(Lit(inputs[0], true));
        clause.push_back(Lit(inputs[1], true));
        printer->create(false, clause);

        // (!u | a)
        clause.clear();
        clause.push_back(Lit(start, true));
        clause.push_back(Lit(inputs[0], false));
        printer->create(false, clause);

        // (!u | b)
        clause.clear();
        clause.push_back(Lit(start, true));
        clause.push_back(Lit(inputs[1], false));
        printer->create(false, clause);

        // XOR -> !s a b
        createXOR(printer, output, inputs[0], inputs[1]);
#else
!a !b  s
!a     s  u
 a !b  s
 a  b !s
 a       !u
    b    !u
#endif


    // Full adder x30
    for (unsigned i = 1; i < 31; i++) {
#ifdef XOR_SUPPORT
        // (u | !a | !b)
        clause.clear();
        clause.push_back(Lit(start + i, false));
        clause.push_back(Lit(inputs[0] + i, true));
        clause.push_back(Lit(inputs[1] + i, true));
        printer->create(false, clause);

        // (u | !a | !c)
        clause.clear();
        clause.push_back(Lit(start + i, false));
        clause.push_back(Lit(inputs[0] + i, true));
        clause.push_back(Lit(start - 1 + i, true));
        printer->create(false, clause);

        // (!u | a | b)
        clause.clear();
        clause.push_back(Lit(start + i, true));
        clause.push_back(Lit(inputs[0] + i, false));
        clause.push_back(Lit(inputs[1] + i, false));
        printer->create(false, clause);

        // (!u | a | c)
        clause.clear();
        clause.push_back(Lit(start + i, true));
        clause.push_back(Lit(inputs[0] + i, false));
        clause.push_back(Lit(start - 1 + i, false));
        printer->create(false, clause);

        // (u | !b | !c)
        clause.clear();
        clause.push_back(Lit(start + i, false));
        clause.push_back(Lit(inputs[1] + i, true));
        clause.push_back(Lit(start - 1 + i, true));
        printer->create(false, clause);

        // (!u | b | c)
        clause.clear();
        clause.push_back(Lit(start + i, true));
        clause.push_back(Lit(inputs[1] + i, false));
        clause.push_back(Lit(start - 1 + i, false));
        printer->create(false, clause);

        // XOR -> !s a b c
        createXOR(printer, output + i, inputs[0] + i, inputs[1] + i, start - 1 + i);
#else
!a !b !c  s
!a !b        u    x
!a  b !c !s
!a  b  c  s
 a !b !c !s
 a !b  c  s
 a  b  c !s
 a  b       !u    x
      !c  s  u
       c !s !u
#endif
    }

    // Final adder (without carry calculation)
    createXOR(printer, output + 31, inputs[0] + 31, inputs[1] + 31, start + 30);
}
