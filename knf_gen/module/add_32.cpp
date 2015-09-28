#include "add_32.h"

#include "add_half_1.h"
#include "add_full_1.h"
#include "clausecreator.h"

#include "../common/solvertools.h"

using std::vector;
using namespace CMSat;

unsigned Add_32::stats[STATS_LENGTH];

Add_32::Add_32() : Modul(32, 2, 1) {
    inputs.push_back(0);
    inputs.push_back(32);
    start = 64;
    output = 95;
}

Add_32::~Add_32() {
}

unsigned* Add_32::getStats() {
    return stats;
}

void Add_32::create(Printer* printer) {
    printer->newModul(10, "Add_32", this);

    vector<unsigned> subinputs;

    // Half adder
    subinputs.clear();
    subinputs.push_back(inputs[0]);
    subinputs.push_back(inputs[1]);
    Add_Half_1 add_half;
    add_half.setInputs(subinputs);
    add_half.setStart(start);
    add_half.setOutput(output);
    add_half.create(printer);

    // Full adder x30
    for (unsigned i = 1; i < 31; i++) {
        subinputs.clear();
        subinputs.push_back(inputs[0] + i);
        subinputs.push_back(inputs[1] + i);
        subinputs.push_back(start - 1 + i);
        Add_Full_1 add_full;
        add_full.setInputs(subinputs);
        add_full.setStart(start + i);
        add_full.setOutput(output + i);
        add_full.create(printer);
    }

    // Final adder (without carry calculation)
    // XOR ->               !s_out            a_in            b_in        c_in
    createXOR(printer, output + 31, inputs[0] + 31, inputs[1] + 31, start + 30);

#ifdef ADDITIONAL_CLAUSES
    ClauseCreator cc(printer);
    //                c_out[0]   c_out[1]s_out[0]    s_out[1]    a_in[0]        a_in[1]    b_in[0]        b_in[1]
    cc.setLiterals(8,    start, start + 1, output, output + 1, inputs[0], inputs[0] + 1, inputs[1], inputs[1] + 1);
    cc.printClause(8,    CC_DC,         0,  CC_DC,      CC_DC,         1,             1,     CC_DC,         CC_DC);
    cc.printClause(8,    CC_DC,         0,  CC_DC,      CC_DC,         1,         CC_DC,     CC_DC,             1);
    cc.printClause(8,    CC_DC,         0,  CC_DC,      CC_DC,     CC_DC,             1,         1,         CC_DC);
    cc.printClause(8,    CC_DC,         0,  CC_DC,      CC_DC,     CC_DC,         CC_DC,         1,             1);
    cc.printClause(8,    CC_DC,         1,  CC_DC,      CC_DC,         0,             0,         0,         CC_DC);
    cc.printClause(8,    CC_DC,         1,  CC_DC,      CC_DC,         0,         CC_DC,         0,             0);
    cc.printClause(8,    CC_DC,         0,  CC_DC,          0,         1,         CC_DC,     CC_DC,         CC_DC);
    cc.printClause(8,    CC_DC,         0,      0,      CC_DC,     CC_DC,             1,     CC_DC,         CC_DC);
    cc.printClause(8,        1,     CC_DC,      1,      CC_DC,     CC_DC,         CC_DC,         0,         CC_DC);
    cc.printClause(8,    CC_DC,         0,      0,      CC_DC,     CC_DC,         CC_DC,     CC_DC,             1);
    cc.printClause(8,    CC_DC,         0,      0,          0,     CC_DC,         CC_DC,     CC_DC,         CC_DC);
    cc.printClause(8,        0,         0,      0,      CC_DC,     CC_DC,             0,     CC_DC,             0);
    cc.printClause(8,    CC_DC,         0,  CC_DC,          0,     CC_DC,         CC_DC,         1,         CC_DC);
    cc.printClause(8,    CC_DC,     CC_DC,      0,          1,     CC_DC,             1,     CC_DC,             0);
    cc.printClause(8,    CC_DC,     CC_DC,      0,          1,     CC_DC,             0,     CC_DC,             1);
    cc.printClause(8,    CC_DC,     CC_DC,      0,          0,     CC_DC,             0,     CC_DC,             0);

    // -40 -71 74 104 105 0
    //                 c_out[6]   c_out[9]    s_out[8]    s_out[9]        b_in[7]
    cc.setLiterals(5, start + 6, start + 9, output + 8, output + 9, inputs[1] + 7);
    cc.printClause(5,         0,         1,          1,          1,             0);

    // -17 -26 -87 90 119 120 0
    //                 c_out[22]   c_out[25]    s_out[23]    s_out[24]        a_in[16]        a_in[25]
    cc.setLiterals(6, start + 22, start + 25, output + 23, output + 24, inputs[0] + 16, inputs[0] + 25);
    cc.printClause(6,          0,          1,           1,           1,              0,              0);

    // -18 50 -79 111 112 -113 0
    //                 c_out[14]    s_out[15]    s_out[16]    s_out[17]        a_in[17]        b_in[17]
    cc.setLiterals(6, start + 14, output + 15, output + 16, output + 17, inputs[0] + 17, inputs[1] + 17);
    cc.printClause(6,          0,           1,           1,           0,              0,              1);
#endif
}

MU_TEST_C(Add_32::test) {
    unsigned a[] = {1234, 5, 0x80000000, 1, 0xFFFFFFFF, 0x2, 0xFFFFFFFF, 0x1, 0xFFFFFFFF, 0x0};
    unsigned b[] = {1235, 6, 1, 0x80000000, 0x2, 0xFFFFFFFF, 0x1, 0xFFFFFFFF, 0x0, 0xFFFFFFFF};

    for (unsigned t = 0; t < 10; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");

        uint32_t ausgabe = a[t] + b[t];

        solver_writeInt(solver,  0, 32, a[t]);
        solver_writeInt(solver, 32, 32, b[t]);

        Add_32 adder;
        adder.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "Adder UNSAT");
        mu_assert(ausgabe == solver_readInt(solver, 95, 32), "Adder failed");
    }
}
