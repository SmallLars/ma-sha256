#include "constadd_32.h"

#include "clausecreator.h"

#include "const.h"

using namespace CMSat;

ConstAdd_32::ConstAdd_32(uint32_t value) : Modul(32, 1, 1) {
    this->value = value;
    inputs.push_back(0);
    start = 32;
    output = 63;
}

ConstAdd_32::~ConstAdd_32() {
}

void ConstAdd_32::setValue(uint32_t value) {
    this->value = value;
}

void ConstAdd_32::create(Printer* printer) {
    ClauseCreator cc(printer);

    // Half adder
    if ((value & 1) == 0) {
        createFalse(printer, start);
        createEQ(printer, output, inputs[0]);
    } else {
        createEQ(printer, start, inputs[0]);
        createNEQ(printer, output, inputs[0]);
    }

    // Full adder x30
    for (unsigned i = 1; i < 31; i++) {
        if (((value >> i) & 1) == 0) {
            // Half adder
#ifdef XOR_SUPPORT
            // AND ->              c_out           a_in           c_in
            createAND(printer, start + i, inputs[0] + i, start - 1 + i);

            // XOR ->              !s_out           a_in           c_in
            createXOR(printer, output + i, inputs[0] + i, start - 1 + i);
#else
            //                     s_out      c_out           a_in           c_in
            cc.setLiterals(4, output + i, start + i, inputs[0] + i, start - 1 + i);
            cc.printClause(4,          0,     CC_DC,             0,             0);
            cc.printClause(4,          1,         1,             0,         CC_DC);
            cc.printClause(4,          1,     CC_DC,             1,             0);
            cc.printClause(4,          0,     CC_DC,             1,             1);
            cc.printClause(4,      CC_DC,         0,             1,         CC_DC);
            cc.printClause(4,      CC_DC,         0,         CC_DC,             1);
#endif
        } else {
#ifdef XOR_SUPPORT
            // OR ->              c_out           a_in           c_in
            createOR(printer, start + i, inputs[0] + i, start - 1 + i);

            // XOR ->               s_out           a_in           c_in
            createXOR(printer, output + i, inputs[0] + i, start - 1 + i, true);
#else
            //                     s_out      c_out           a_in           c_in
            cc.setLiterals(4, output + i, start + i, inputs[0] + i, start - 1 + i);
            cc.printClause(4,          1,     CC_DC,             0,             0);
            cc.printClause(4,          0,     CC_DC,             0,             1);
            cc.printClause(4,      CC_DC,         1,             0,         CC_DC);
            cc.printClause(4,          1,     CC_DC,             1,             1);
            cc.printClause(4,          0,         0,             1,         CC_DC);
            cc.printClause(4,      CC_DC,         1,         CC_DC,             0);
#endif
        }
    }

    // Final adder (without carry calculation)
    if (((value >> 31) & 1) == 0) {
        // XOR ->               !s_out            a_in        c_in
        createXOR(printer, output + 31, inputs[0] + 31, start + 30);
    } else {
        // XOR ->               !s_out            a_in        c_in
        createXOR(printer, output + 31, inputs[0] + 31, start + 30, true);
    }
}

MU_TEST_C(ConstAdd_32::test) {
    unsigned a[] = {1234, 5, 0x80000000, 1, 0xFFFFFFFF, 0x2, 0xFFFFFFFF, 0x1, 0xFFFFFFFF, 0x0};
    unsigned b[] = {1235, 6, 1, 0x80000000, 0x2, 0xFFFFFFFF, 0x1, 0xFFFFFFFF, 0x0, 0xFFFFFFFF};

    for (unsigned t = 0; t < 10; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");
        solver.set_num_threads(4);

        uint32_t ausgabe = a[t] + b[t];
        uint32_t result = 0;

        Const con(32, a[t]);
        con.append(&solver);

        ConstAdd_32 constadder(b[t]);
        constadder.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "ConstAdd UNSAT");

        for (unsigned i = 94; i >=63; i--) {
            result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 63));
        }

        mu_assert(ausgabe == result, "ConstAdd failed");
    }
}
