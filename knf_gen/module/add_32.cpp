#include "add_32.h"

#include "add_half_1.h"
#include "add_full_1.h"
#include "add_last_1.h"
#include "add_half_2.h"
#include "add_full_2.h"
#include "add_last_2.h"
#include "add_half_3.h"
#include "add_full_3.h"
#include "add_last_3.h"
#include "add_half_4.h"
#include "add_full_4.h"
#include "add_last_4.h"
#include "clausecreator.h"

#include "../common/solvertools.h"

using std::vector;
using namespace CMSat;

unsigned Add_32::stats[STATS_LENGTH];

Add_32::Add_32() : Modul(32, 2, 1) {
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
    Add_Half_1 add_half_1;
    add_half_1.setInputs(subinputs);
    add_half_1.setStart(start);
    add_half_1.setOutput(output);
    add_half_1.create(printer);

    // Full adder x30
    for (unsigned i = 1; i < 31; i++) {
        subinputs.clear();
        subinputs.push_back(inputs[0] + i);
        subinputs.push_back(inputs[1] + i);
        subinputs.push_back(start - 1 + i);
        Add_Full_1 add_full_1;
        add_full_1.setInputs(subinputs);
        add_full_1.setStart(start + i);
        add_full_1.setOutput(output + i);
        add_full_1.create(printer);
    }

    // Last adder
    subinputs.clear();
    subinputs.push_back(inputs[0] + 31);
    subinputs.push_back(inputs[1] + 31);
    subinputs.push_back(start + 30);
    Add_Last_1 add_last_1;
    add_last_1.setInputs(subinputs);
    add_last_1.setOutput(output + 31);
    add_last_1.create(printer);

#ifdef ADDITIONAL_CLAUSES
    // Half adder 2
    subinputs.clear();
    subinputs.push_back(inputs[0]);
    subinputs.push_back(inputs[1]);
    Add_Half_2 add_half_2;
    add_half_2.setInputs(subinputs);
    add_half_2.setStart(start + 1);
    add_half_2.setOutput(output);
    add_half_2.create(printer);

    // Full adder 2 x29
    for (unsigned i = 0; i < 29; i++) {
        subinputs.clear();
        subinputs.push_back(inputs[0] + 1 + i);
        subinputs.push_back(inputs[1] + 1 + i);
        subinputs.push_back(start + i);
        Add_Full_2 add_full;
        add_full.setInputs(subinputs);
        add_full.setStart(start + 2 + i);
        add_full.setOutput(output + 1 + i);
        add_full.create(printer);
    }

    // Last adder 2
    subinputs.clear();
    subinputs.push_back(inputs[0] + 30);
    subinputs.push_back(inputs[1] + 30);
    subinputs.push_back(start + 29);
    Add_Last_2 add_last_2;
    add_last_2.setInputs(subinputs);
    add_last_2.setOutput(output + 30);
    add_last_2.create(printer);

    // Half adder 3
    subinputs.clear();
    subinputs.push_back(inputs[0]);
    subinputs.push_back(inputs[1]);
    Add_Half_3 add_half_3;
    add_half_3.setInputs(subinputs);
    add_half_3.setStart(start + 2);
    add_half_3.setOutput(output);
    add_half_3.create(printer);

    // Full adder 3 x28
    for (unsigned i = 0; i < 28; i++) {
        subinputs.clear();
        subinputs.push_back(inputs[0] + 1 + i);
        subinputs.push_back(inputs[1] + 1 + i);
        subinputs.push_back(start + i);
        Add_Full_3 add_full;
        add_full.setInputs(subinputs);
        add_full.setStart(start + 3 + i);
        add_full.setOutput(output + 1 + i);
        add_full.create(printer);
    }

    // Last adder 3
    subinputs.clear();
    subinputs.push_back(inputs[0] + 29);
    subinputs.push_back(inputs[1] + 29);
    subinputs.push_back(start + 28);
    Add_Last_3 add_last_3;
    add_last_3.setInputs(subinputs);
    add_last_3.setOutput(output + 29);
    add_last_3.create(printer);
/*
    // Half adder 4
    subinputs.clear();
    subinputs.push_back(inputs[0]);
    subinputs.push_back(inputs[1]);
    Add_Half_4 add_half_4;
    add_half_4.setInputs(subinputs);
    add_half_4.setStart(start + 3);
    add_half_4.setOutput(output);
    add_half_4.create(printer);

    // Full adder 4 x27
    for (unsigned i = 0; i < 27; i++) {
        subinputs.clear();
        subinputs.push_back(inputs[0] + 1 + i);
        subinputs.push_back(inputs[1] + 1 + i);
        subinputs.push_back(start + i);
        Add_Full_4 add_full;
        add_full.setInputs(subinputs);
        add_full.setStart(start + 4 + i);
        add_full.setOutput(output + 1 + i);
        add_full.create(printer);
    }

    // Last adder 4
    subinputs.clear();
    subinputs.push_back(inputs[0] + 28);
    subinputs.push_back(inputs[1] + 28);
    subinputs.push_back(start + 27);
    Add_Last_4 add_last_4;
    add_last_4.setInputs(subinputs);
    add_last_4.setOutput(output + 28);
    add_last_4.create(printer);
*/

    ClauseCreator cc(printer);
    //                      65         66      96          97          1              2         33             34
    //                c_out[0]   c_out[1]s_out[0]    s_out[1]    a_in[0]        a_in[1]    b_in[0]        b_in[1]
    cc.setLiterals(8,    start, start + 1, output, output + 1, inputs[0], inputs[0] + 1, inputs[1], inputs[1] + 1);
    cc.printClause(8,        0,         0,      0,      CC_DC,     CC_DC,             0,     CC_DC,             0);

    for (unsigned i = 0; i < 30; i++) {
        // 64 94 -95 -126 0
        // 64 -94 95 126 0
        //                 c_out[0]       c_out[1]        s_out[1]            a_in[2]            b_in[2]
        cc.setLiterals(5, start + i, start + 1 + i, output + 1 + i, inputs[0] + 2 + i, inputs[1] + 2 + i);
        cc.printClause(5,         1,             0,              0,             CC_DC,                 1);
        cc.printClause(5,         0,             1,              1,             CC_DC,                 1);
        cc.printClause(5,         1,             0,              0,                 1,             CC_DC);
        cc.printClause(5,         0,             1,              1,                 1,             CC_DC);
    }

    for (unsigned i = 0; i < 22; i++) {
        // -17 -26 -87 90 119 120 0
        //                     c_out[6]       c_out[9]        s_out[7]        s_out[8]         a_in[0]            a_in[9]         b_in[0]            b_in[9]
        cc.setLiterals(8, start + 6 + i, start + 9 + i, output + 7 + i, output + 8 + i, inputs[0] +  i, inputs[0] + 9 + i, inputs[1] +  i, inputs[1] + 9 + i);
        cc.printClause(8,             0,             1,              1,              1,          CC_DC,             CC_DC,              0,                 0);
        cc.printClause(8,             0,             1,              1,              1,              0,                 0,          CC_DC,             CC_DC);
        cc.printClause(8,             0,             1,              1,              1,              0,             CC_DC,          CC_DC,                 0);
        cc.printClause(8,             0,             1,              1,              1,          CC_DC,                 0,              0,             CC_DC);
    }

    for (unsigned i = 0; i < 30; i++) {
        // 12 44 74 105 -106 -107 0
        //                 c_out[0]    s_out[0]        s_out[1]        s_out[2]            a_in[2]            b_in[2]
        cc.setLiterals(6, start + i, output + i, output + 1 + i, output + 2 + i, inputs[0] + 2 + i, inputs[1] + 2 + i);
        cc.printClause(6,         1,          1,              0,              0,                 1,                 1);
    }

    for (unsigned i = 0; i < 29; i++) {
        // -32 64 -94 95 126 0
        // 32 64 94 -95 125 -126 0
        // -32 64 -93 95 125 126 0
        //                 c_out[0]       c_out[1]       c_out[2]        s_out[1]        s_out[2]            a_in[3]            b_in[3]
        cc.setLiterals(7, start + i, start + 1 + i, start + 2 + i, output + 1 + i, output + 2 + i, inputs[0] + 3 + i, inputs[1] + 3 + i);
        cc.printClause(7,     CC_DC,             0,             1,          CC_DC,              1,                 0,                 1);
        cc.printClause(7,     CC_DC,             1,             0,              1,              0,                 1,                 1);
        cc.printClause(7,         0,         CC_DC,             1,              1,              1,                 0,                 1);
        cc.printClause(7,     CC_DC,             0,             1,          CC_DC,              1,                 1,                 0);
        cc.printClause(7,         0,         CC_DC,             1,              1,              1,                 1,                 0);
    }

    for (unsigned i = 0; i < 26; i++) {
        // -38 86 -91 -118 -119 -120 -121 -122 0    ->    38 is irrelevant (b_in[5])
        //                 c_out[0]       c_out[5]        s_out[1]        s_out[2]        s_out[3]        s_out[4]        s_out[5]
        cc.setLiterals(7, start + i, start + 5 + i, output + 1 + i, output + 2 + i, output + 3 + i, output + 4 + i, output + 5 + i);
        cc.printClause(7,         1,             0,              0,              0,              0,              0,              0);
    }

    for (unsigned i = 0; i < 30; i++) {
        //                   65             66             67              97              98
        //             c_out[0]       c_out[1]       c_out[2]        s_out[1]        s_out[2]
        cc.setLiterals(6, start + i, start + 1 + i, start + 2 + i, output + 1 + i, output + 2 + i);
        cc.printClause(6,         1,             0,         CC_DC,              0,              1);
        cc.printClause(6,         0,             1,         CC_DC,              1,              0);
        if (i == 29) break;
        cc.printClause(6,         1,             0,             1,              0,          CC_DC);
    }

    for (unsigned i = 0; i < 30; i++) {
        //                       65         66              97              98                  3                 35
        //                 c_out[0]   c_out[1]        s_out[1]        s_out[2]            a_in[2]            b_in[2]
        cc.setLiterals(6, start + i, start + 1 + i, output + 1 + i, output + 2 + i, inputs[0] + 2 + i, inputs[1] + 2 + i);
        cc.printClause(6,         1,             0,              0,              1,                 0,             CC_DC);
        cc.printClause(6,         1,             0,              0,              1,             CC_DC,                 0);
    }

    for (unsigned i = 0; i < 29; i++) {
        //                       65         67              97              98                  4                 36
        //                 c_out[0]   c_out[2]        s_out[1]        s_out[2]            a_in[3]            b_in[3]
        cc.setLiterals(6, start + i, start + 2 + i, output + 1 + i, output + 2 + i, inputs[0] + 3 + i, inputs[1] + 3 + i);
        cc.printClause(6,         0,             1,              1,              1,                 1,             CC_DC);
        cc.printClause(6,         0,             1,              1,              1,             CC_DC,                 1);
    }

    for (unsigned i = 0; i < 29; i++) {
        //                       65         67          96              97                  3                 35
        //                 c_out[0]   c_out[2]    s_out[0]        s_out[1]            a_in[2]            b_in[2]
        cc.setLiterals(6, start + i, start + 2 + i, output + i, output + 1 + i, inputs[0] + 2 + i, inputs[1] + 2 + i);
        cc.printClause(6,         1,             0,          0,              0,                 1,             CC_DC);
        cc.printClause(6,         1,             0,          0,              0,             CC_DC,                 1);
    }

    for (unsigned i = 0; i < 30; i++) {
        //                       65         66              97              1             33
        //                 c_out[0]   c_out[1]        s_out[1]        a_in[0]        b_in[0]
        cc.setLiterals(6, start + i, start + 1 + i, output + 1 + i, inputs[0] + i, inputs[1] + i);
        cc.printClause(6,         0,             1,              1,             0,             0);
        cc.printClause(6,         1,             0,              0,             1,             1);
    }

    for (unsigned i = 0; i < 30; i++) {
        //                       65         66          96              97              1             33
        //                 c_out[0]   c_out[1]    s_out[0]        s_out[1]        a_in[0]        b_in[0]
        cc.setLiterals(6, start + i, start + 1 + i, output + i, output + 1 + i, inputs[0] + i, inputs[1] + i);
        cc.printClause(6,         0,             1,          1,              1,             0,         CC_DC);
        cc.printClause(6,         0,             1,          1,              1,         CC_DC,             0);
    }

    for (unsigned i = 0; i < 29; i++) {
        //                   65             67              97              98              99
        //             c_out[0]       c_out[2]        s_out[1]        s_out[2]        s_out[3]
        cc.setLiterals(6, start + i, start + 2 + i, output + 1 + i, output + 2 + i, output + 3 + i);
        cc.printClause(6,         0,             1,              1,              1,              0);
    }

    for (unsigned i = 0; i < 29; i++) {
        //                   65             66             67              97              98
        //             c_out[0]       c_out[1]       c_out[2]        s_out[1]        s_out[2]
        cc.setLiterals(6, start + i, start + 1 + i, start + 2 + i, output + 1 + i, output + 2 + i);
        cc.printClause(6,         0,             0,             1,              1,              1);
    }
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
