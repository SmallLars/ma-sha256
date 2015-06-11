#include "maj_32.h"

#include "clausecreator.h"

#include "const_32.h"
using namespace CMSat;

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

void Maj_32::test() {
    unsigned a[] = {0xabcdef98};
    unsigned b[] = {0x651d8fa1};
    unsigned c[] = {0x456af012};

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");
        solver.set_num_threads(4);

        uint32_t ausgabe = (a[t] & b[t]) ^ (a[t] & c[t]) ^ (b[t] & c[t]);
        uint32_t result = 0;

        Const_32 ca(a[t]);
        ca.setOutput(0);
        ca.append(&solver);

        ca.setValue(b[t]);
        ca.setOutput(32);
        ca.append(&solver);

        ca.setValue(c[t]);
        ca.setOutput(64);
        ca.append(&solver);

        Maj_32 maj;
        maj.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "MAJ UNSAT");

        for (unsigned i = 127; i >=96; i--) {
            result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 96));
        }

        mu_assert(ausgabe == result, "MAJ failed");
    }
}
