#include "constadd_32.h"

#include "add_half_1.h"
#include "clausecreator.h"

#include "../common/solvertools.h"

using std::vector;
using namespace CMSat;

unsigned ConstAdd_32::stats[STATS_LENGTH];

ConstAdd_32::ConstAdd_32(uint32_t value) : Modul(32, 1, 1) {
    this->value = value;
    output = 63;
}

ConstAdd_32::~ConstAdd_32() {
}

void ConstAdd_32::setValue(uint32_t value) {
    this->value = value;
}

unsigned* ConstAdd_32::getStats() {
    return stats;
}

void ConstAdd_32::create(Printer* printer) {
    printer->newModul(10, "ConstAdd_32", this);

    vector<unsigned> subinputs;
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
            subinputs.clear();
            subinputs.push_back(inputs[0] + i);
            subinputs.push_back(start - 1 + i);
            Add_Half_1 add_half;
            add_half.setInputs(subinputs);
            add_half.setStart(start + i);
            add_half.setOutput(output + i);
            add_half.create(printer);
        } else {
#ifdef XOR_SUPPORT
            // OR ->              c_out           a_in           c_in
            createOR(printer, start + i, inputs[0] + i, start - 1 + i);

            // XOR ->               s_out           a_in           c_in
            createXOR(printer, output + i, inputs[0] + i, start - 1 + i, true);
#else
            //                    c_out       s_out           a_in           c_in
            cc.setLiterals(4, start + i, output + i, inputs[0] + i, start - 1 + i);
            cc.printClause(4,     CC_DC,          1,             0,             0);
            cc.printClause(4,         1,          1,         CC_DC,         CC_DC);
            cc.printClause(4,         1,      CC_DC,             0,         CC_DC);
            cc.printClause(4,     CC_DC,          0,             1,             0);
            cc.printClause(4,         0,          0,         CC_DC,             1);
            cc.printClause(4,         0,      CC_DC,             1,             1);
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

    if (value == 0) return;

#ifdef ADDITIONAL_CLAUSES
    for (unsigned i = 0; i < 32; i++) {
        if ((value >> i) & 1) {
            if (i < 31) {
              //                         a_in      c_out
              cc.setLiterals(2, inputs[0] + i, start + i);
              cc.printClause(2,             1,         0);
            }
            break;
        }
        // Fix carry bits and equal output if constant starts with zeros from LSB
        createFalse(printer, start + i);
        createEQ(printer, output + i, inputs[0] + i);
    }

    // A single 1 Bit can cause the following carry bits to be 1 on a long row of constant 1 bits
    for (unsigned i = 0; i < 30; i++) {
        if ((value >> i) & 1) {
            for (unsigned j = i + 1; j < 31; j++) {
                if ((value >> j) & 1) {
                    //                         a_in      c_out
                    cc.setLiterals(2, inputs[0] + i, start + j);
                    cc.printClause(2,             0,         1);
                } else {
                    break;
                }
            }
        }
    }

    // A single 0 Bit can cause the following carry bits to be 0 on a long row of constant 0 bits
    for (unsigned i = 0; i < 30; i++) {
        if ((value >> i) & 1) continue;

        for (unsigned j = i + 1; j < 31; j++) {
            if ((value >> j) & 1) break;

            //                         a_in      c_out
            cc.setLiterals(2, inputs[0] + i, start + j);
            cc.printClause(2,             1,         0);
        }
    }
#endif
}

MU_TEST_C(ConstAdd_32::test) {
    unsigned a[] = {1234, 5, 0x80000000, 1, 0xFFFFFFFF, 0x2, 0xFFFFFFFF, 0x1, 0xFFFFFFFF, 0x0};
    unsigned b[] = {1235, 6, 1, 0x80000000, 0x2, 0xFFFFFFFF, 0x1, 0xFFFFFFFF, 0x0, 0xFFFFFFFF};

    for (unsigned t = 0; t < 10; t++) {
        SATSolver solver;
        solver.set_verbosity(0);
        solver.log_to_file("test.log");

        uint32_t ausgabe = a[t] + b[t];

        solver_writeInt(solver, 0, 32, a[t]);

        ConstAdd_32 constadder(b[t]);
        constadder.append(&solver);

        lbool ret = solver.solve();
        if (ret != l_True) printf("\n%u + %u = %u\n", a[t], b[t], ausgabe);
        mu_assert(ret == l_True, "ConstAdd UNSAT");
        mu_assert(ausgabe == solver_readInt(solver, 63, 32), "ConstAdd failed");
    }
}
