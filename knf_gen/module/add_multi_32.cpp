#include "add_multi_32.h"

#include "clausecreator.h"

#include "const.h"

using namespace CMSat;

Add_Multi_32::Add_Multi_32() : Modul(32, 2, 1) {
    inputs.push_back(0);
    inputs.push_back(32);
    start = 64;
    output = 80;
}

Add_Multi_32::~Add_Multi_32() {
}

void Add_Multi_32::create(Printer* printer) {
    ClauseCreator cc(printer);

    // Half adder
#ifdef XOR_SUPPORT
    // AND ->          c_out       a_in       b_in
    createAND(printer, start, inputs[0], inputs[1]);

    // XOR ->          !s_out       a_in       b_in
    createXOR(printer, output, inputs[0], inputs[1]);
#else
    //                 s_out  c_out       a_in       b_in
    cc.setLiterals(4, output, start, inputs[0], inputs[1]);
    cc.printClause(4,      0, CC_DC,         1,         1);
    cc.printClause(4,      0,     0,     CC_DC,     CC_DC);
    cc.printClause(4,  CC_DC,     0,     CC_DC,         1);
    cc.printClause(4,  CC_DC,     1,         0,         0);
    cc.printClause(4,      1, CC_DC,         1,         0);
    cc.printClause(4,      1, CC_DC,         0,         1);
#endif

    // Full 2bit adder x15
    for (unsigned i = 1; i < 16; i++) {
        unsigned sum1 = i * 2;
        unsigned sum0 = sum1 - 1;
#ifdef XOR_SUPPORT
        //                    c_out         s1_out             a1_in             a0_in             b1_in             b0_in           c_in
        cc.setLiterals(7, start + i, output + sum1, inputs[0] + sum1, inputs[0] + sum0, inputs[1] + sum1, inputs[1] + sum0, start - 1 + i);
        cc.printClause(7,         1,             1,                0,            CC_DC,            CC_DC,            CC_DC,         CC_DC);
        cc.printClause(7,         1,             1,            CC_DC,            CC_DC,                0,            CC_DC,         CC_DC);
        cc.printClause(7,     CC_DC,             0,                1,                1,                1,                1,         CC_DC);
        cc.printClause(7,         1,         CC_DC,                0,            CC_DC,                0,            CC_DC,         CC_DC);
        cc.printClause(7,         1,             1,            CC_DC,            CC_DC,            CC_DC,                0,             0);
        cc.printClause(7,     CC_DC,             0,                1,                1,                1,            CC_DC,             1);
        cc.printClause(7,         1,         CC_DC,            CC_DC,            CC_DC,                0,                0,             0);
        cc.printClause(7,         1,         CC_DC,                0,            CC_DC,            CC_DC,                0,             0);
        cc.printClause(7,         1,             1,            CC_DC,                0,            CC_DC,            CC_DC,             0);
        cc.printClause(7,     CC_DC,             0,                1,            CC_DC,                1,                1,             1);
        cc.printClause(7,         1,         CC_DC,            CC_DC,                0,                0,            CC_DC,             0);
        cc.printClause(7,         1,         CC_DC,                0,                0,            CC_DC,            CC_DC,             0);
        cc.printClause(7,         1,             1,            CC_DC,                0,            CC_DC,                0,         CC_DC);
        cc.printClause(7,         1,         CC_DC,                0,                0,            CC_DC,                0,         CC_DC);
        cc.printClause(7,         1,         CC_DC,            CC_DC,                0,                0,                0,         CC_DC);
        cc.printClause(7,         0,             0,            CC_DC,                1,            CC_DC,                1,         CC_DC);
        cc.printClause(7,         0,         CC_DC,                1,                1,            CC_DC,                1,         CC_DC);
        cc.printClause(7,         0,         CC_DC,            CC_DC,                1,                1,                1,         CC_DC);
        cc.printClause(7,         0,         CC_DC,                1,            CC_DC,                1,            CC_DC,         CC_DC);
        cc.printClause(7,         0,         CC_DC,                1,                1,            CC_DC,            CC_DC,             1);
        cc.printClause(7,         0,         CC_DC,            CC_DC,                1,                1,            CC_DC,             1);
        cc.printClause(7,     CC_DC,             1,                0,            CC_DC,                0,                0,             0);
        cc.printClause(7,         0,             0,                1,            CC_DC,            CC_DC,            CC_DC,         CC_DC);
        cc.printClause(7,         0,             0,            CC_DC,            CC_DC,                1,            CC_DC,         CC_DC);
        cc.printClause(7,         0,             0,            CC_DC,                1,            CC_DC,            CC_DC,             1);
        cc.printClause(7,         0,         CC_DC,                1,            CC_DC,            CC_DC,                1,             1);
        cc.printClause(7,         0,         CC_DC,            CC_DC,            CC_DC,                1,                1,             1);
        cc.printClause(7,     CC_DC,             1,                0,                0,                0,            CC_DC,             0);
        cc.printClause(7,         0,             0,            CC_DC,            CC_DC,            CC_DC,                1,             1);
        cc.printClause(7,     CC_DC,             1,                0,                0,                0,                0,         CC_DC);

        // XOR ->                !s0_out             a0_in             b0_in           c_in
        createXOR(printer, output + sum0, inputs[0] + sum0, inputs[1] + sum0, start - 1 + i);
#else
/*
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
*/
        //                    c_out         s1_out         s0_out             a1_in             a0_in             b1_in             b0_in           c_in
//        cc.setLiterals(7, start + i, output + sum1, output + sum0, inputs[0] + sum1, inputs[0] + sum0, inputs[1] + sum1, inputs[1] + sum0, start - 1 + i);
#endif
    }

    // Final adder (without carry calculation)
    // XOR ->               !s_out            a_in            b_in        c_in
    createXOR(printer, output + 31, inputs[0] + 31, inputs[1] + 31, start + 15);
}

MU_TEST_C(Add_Multi_32::test) {
    unsigned a[] = {1234, 5, 0x80000000, 1, 0xFFFFFFFF, 0x2, 0xFFFFFFFF, 0x1, 0xFFFFFFFF, 0x0};
    unsigned b[] = {1235, 6, 1, 0x80000000, 0x2, 0xFFFFFFFF, 0x1, 0xFFFFFFFF, 0x0, 0xFFFFFFFF};

    for (unsigned t = 0; t < 10; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");
        solver.set_num_threads(4);

        uint32_t ausgabe = a[t] + b[t];
        uint32_t result = 0;

        Const con(32, a[t]);
        con.setOutput(0);
        con.append(&solver);

        con.setValue(b[t]);
        con.setOutput(32);
        con.append(&solver);

        Add_Multi_32 adder;
        adder.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "Adder UNSAT");

        for (unsigned i = 111; i >=80; i--) {
            result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 80));
        }

        mu_assert(ausgabe == result, "Adder failed");
    }
}
