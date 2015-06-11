#include "adder_32.h"

#include "clausecreator.h"

#include "const_32.h"
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
    ClauseCreator cc(printer);

    // Half adder
#ifdef XOR_SUPPORT
    //                c_out       a_in       b_in
    cc.setLiterals(3, start, inputs[0], inputs[1]);
    cc.printClause(3,     1,         0,         0);
    cc.printClause(3,     0,         1,     CC_DC);
    cc.printClause(3,     0,     CC_DC,         1);

    // XOR ->          !s_out       a_in       b_in
    createXOR(printer, output, inputs[0], inputs[1]);
#else
    //                 s_out  c_out       a_in       b_in
    cc.setLiterals(4, output, start, inputs[0], inputs[1]);
    cc.printClause(4,      1, CC_DC,         0,         0);
    cc.printClause(4,      1,     1,         0,     CC_DC);
    cc.printClause(4,      1, CC_DC,         1,         0);
    cc.printClause(4,      0, CC_DC,         1,         1);
    cc.printClause(4,  CC_DC,     0,         1,     CC_DC);
    cc.printClause(4,  CC_DC,     0,     CC_DC,         1);
#endif

    // Full adder x30
    for (unsigned i = 1; i < 31; i++) {
#ifdef XOR_SUPPORT
        //                    c_out           a_in           b_in           c_in
        cc.setLiterals(4, start + i, inputs[0] + i, inputs[1] + i, start - 1 + i);
        cc.printClause(4,         1,             0,             0,         CC_DC);
        cc.printClause(4,         1,             0,         CC_DC,             0);
        cc.printClause(4,         0,             1,             1,         CC_DC);
        cc.printClause(4,         0,             1,         CC_DC,             1);
        cc.printClause(4,         1,         CC_DC,             0,             0);
        cc.printClause(4,         0,         CC_DC,             1,             1);

        // XOR ->              !s_out           a_in           b_in           c_in
        createXOR(printer, output + i, inputs[0] + i, inputs[1] + i, start - 1 + i);
#else
        //                     s_out      c_out           a_in           b_in           c_in
        cc.setLiterals(5, output + i, start + i, inputs[0] + i, inputs[1] + i, start - 1 + i);
        cc.printClause(5,          1,     CC_DC,             0,             0,             0);
        cc.printClause(5,      CC_DC,         1,             0,             0,         CC_DC);
        cc.printClause(5,          0,     CC_DC,             0,             1,             0);
        cc.printClause(5,          1,     CC_DC,             0,             1,             1);
        cc.printClause(5,          0,     CC_DC,             1,             0,             0);
        cc.printClause(5,          1,     CC_DC,             1,             0,             1);
        cc.printClause(5,          0,     CC_DC,             1,             1,             1);
        cc.printClause(5,      CC_DC,         0,             1,             1,         CC_DC);
        cc.printClause(5,          1,         1,         CC_DC,         CC_DC,             0);
        cc.printClause(5,          0,         0,         CC_DC,         CC_DC,             1);
#endif
    }

    // Final adder (without carry calculation)
    // XOR ->               !s_out            a_in            b_in        c_in
    createXOR(printer, output + 31, inputs[0] + 31, inputs[1] + 31, start + 30);
}

MU_TEST_C(Adder_32::test) {
    unsigned a[] = {1234, 5};
    unsigned b[] = {1235, 6};

    for (unsigned t = 0; t < 2; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");
        solver.set_num_threads(4);

        uint32_t ausgabe = a[t] + b[t];
        uint32_t result = 0;

        Const_32 ca(a[t]);
        ca.setOutput(0);
        ca.append(&solver);

        ca.setValue(b[t]);
        ca.setOutput(32);
        ca.append(&solver);

        Adder_32 adder;
        adder.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "Adder UNSAT");

        for (unsigned i = 126; i >=95; i--) {
            result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 95));
        }

        mu_assert(ausgabe == result, "Adder failed");
    }
}
