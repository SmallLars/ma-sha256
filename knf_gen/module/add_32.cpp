#include "add_32.h"

#include "add_half_1.h"
#include "add_full_1.h"
#include "add_full_2.h"
#include "add_full_3.h"
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

    ClauseCreator cc(printer);
    //                      65         66      96          97          1              2         33             34
    //                c_out[0]   c_out[1]s_out[0]    s_out[1]    a_in[0]        a_in[1]    b_in[0]        b_in[1]
    cc.setLiterals(8,    start, start + 1, output, output + 1, inputs[0], inputs[0] + 1, inputs[1], inputs[1] + 1);
    cc.printClause(8,    CC_DC,         0,  CC_DC,      CC_DC,         1,             1,     CC_DC,         CC_DC);  //3
    cc.printClause(8,    CC_DC,         0,  CC_DC,      CC_DC,         1,         CC_DC,     CC_DC,             1);  //3
    cc.printClause(8,    CC_DC,         0,  CC_DC,      CC_DC,     CC_DC,             1,         1,         CC_DC);  //3
    cc.printClause(8,    CC_DC,         0,  CC_DC,      CC_DC,     CC_DC,         CC_DC,         1,             1);  //3
    cc.printClause(8,    CC_DC,         0,  CC_DC,          0,         1,         CC_DC,     CC_DC,         CC_DC);  //3
    cc.printClause(8,    CC_DC,         0,      0,      CC_DC,     CC_DC,             1,     CC_DC,         CC_DC);  //3
    cc.printClause(8,        1,     CC_DC,      1,      CC_DC,     CC_DC,         CC_DC,         0,         CC_DC);  //3
    cc.printClause(8,    CC_DC,         0,      0,      CC_DC,     CC_DC,         CC_DC,     CC_DC,             1);  //3
    cc.printClause(8,    CC_DC,         0,      0,          0,     CC_DC,         CC_DC,     CC_DC,         CC_DC);  //3
    cc.printClause(8,    CC_DC,         0,  CC_DC,          0,     CC_DC,         CC_DC,         1,         CC_DC);  //3
    cc.printClause(8,    CC_DC,         1,  CC_DC,      CC_DC,         0,             0,         0,         CC_DC);  //4
    cc.printClause(8,    CC_DC,         1,  CC_DC,      CC_DC,         0,         CC_DC,         0,             0);  //4
    cc.printClause(8,    CC_DC,     CC_DC,      0,          1,     CC_DC,             1,     CC_DC,             0);  //4
    cc.printClause(8,    CC_DC,     CC_DC,      0,          1,     CC_DC,             0,     CC_DC,             1);  //4
    cc.printClause(8,    CC_DC,     CC_DC,      0,          0,     CC_DC,             0,     CC_DC,             0);  //4
    cc.printClause(8,    CC_DC,     CC_DC,      0,          1,     CC_DC,             1,     CC_DC,             0);  //4
    cc.printClause(8,    CC_DC,     CC_DC,      0,          1,     CC_DC,             0,     CC_DC,             1);  //4
    cc.printClause(8,    CC_DC,     CC_DC,      0,          0,     CC_DC,             0,     CC_DC,             0);  //4
    cc.printClause(8,        0,         0,      0,      CC_DC,     CC_DC,             0,     CC_DC,             0);  //5

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

    for (unsigned i = 0; i < 29; i++) {
      // 66 -68 -98 -99 0
      //                 c_out[0]       c_out[2]         s_out[1]        s_out[2]
      cc.setLiterals(4, start + i, start + 2 + i,  output + 1 + i, output + 2 + i);
      cc.printClause(4,         1,             0,               0,              0);
    }

    for (unsigned i = 0; i < 29; i++) {
      // -3 -65 67 97 0
      // -4 -66 68 98 0
      // -5 -67 69 99 0
      // -6 -68 70 100 0
      // -7 -69 71 101 0
      // -8 -70 72 102 0
      // -9 -71 73 103 0
      // -10 -72 74 104 0
      // -11 -73 75 105 0
      // -12 -74 76 106 0
      // -13 -75 77 107 0
      // -14 -76 78 108 0
      // -15 -77 79 109 0
      // -16 -78 80 110 0
      // -17 -79 81 111 0
      // -18 -80 82 112 0
      // -19 -81 83 113 0
      // -20 -82 84 114 0
      // -21 -83 85 115 0
      // -22 -84 86 116 0
      // -23 -85 87 117 0
      // -24 -86 88 118 0
      // -25 -87 89 119 0
      // -29 -91 93 123 0
      // -31 -93 95 125 0
      //                 c_out[0]       c_out[2]        s_out[1]            a_in[2]            b_in[2]
      cc.setLiterals(5, start + i, start + 2 + i, output + 1 + i, inputs[0] + 2 + i, inputs[1] + 2 + i);
      cc.printClause(5,         0,             1,              1,             CC_DC,                 0);
      cc.printClause(5,         0,             1,              1,                 0,             CC_DC);
    }

    for (unsigned i = 0; i < 30; i++) {
      // 3 35 65 -97 -98 0
      //                 c_out[0]        s_out[1]        s_out[2]            a_in[2]            b_in[2] 
      cc.setLiterals(5, start + i, output + 1 + i, output + 2 + i, inputs[0] + 2 + i, inputs[1] + 2 + i);
      cc.printClause(5,         1,              0,              0,                 1,                 1);
    }

    for (unsigned i = 0; i < 28; i++) {
      // -40 -71 74 104 105 0
      // -43 -74 77 107 108 0
      //                 c_out[0]       c_out[3]        s_out[2]        s_out[3]            a_in[1]            b_in[1]
      cc.setLiterals(6, start + i, start + 3 + i, output + 2 + i, output + 3 + i, inputs[0] + 1 + i, inputs[1] + 1 + i);
      cc.printClause(6,         0,             1,              1,              1,             CC_DC,                 0);
      cc.printClause(6,         0,             1,              1,              1,                 0,             CC_DC);
    }

    for (unsigned i = 0; i < 28; i++) {
      // -11 -72 75 104 105 0
      // -12 -73 76 105 106 0
      // -13 -74 77 106 107 0
      // -14 -75 78 107 108 0
      // -15 -76 79 108 109 0
      // -16 -77 80 109 110 0
      // -17 -78 81 110 111 0
      // -18 -79 82 111 112 0
      // -21 -82 85 114 115 0
      // -22 -83 86 115 116 0
      // -23 -84 87 116 117 0
      // -24 -85 88 117 118 0
      // -25 -86 89 118 119 0
      // -26 -87 90 119 120 0
      // -27 -88 91 120 121 0
      // -29 -90 93 122 123 0
      // -30 -91 94 123 124 0
      //                 c_out[0]       c_out[3]        s_out[1]        s_out[2]            a_in[3]            b_in[3]
      cc.setLiterals(6, start + i, start + 3 + i, output + 1 + i, output + 2 + i, inputs[0] + 3 + i, inputs[1] + 3 + i);
      cc.printClause(6,         0,             1,              1,              1,             CC_DC,                 0);
      cc.printClause(6,         0,             1,              1,              1,                 0,             CC_DC);
    }

    for (unsigned i = 0; i < 30; i++) {
      // -3 35 58 -65 97 -98 0    ->    58 is irrelevant (b_in[25])
      //                 c_out[0]        s_out[1]        s_out[2]            a_in[2]            b_in[2]
      cc.setLiterals(5, start + i, output + 1 + i, output + 2 + i, inputs[0] + 2 + i, inputs[1] + 2 + i);
      cc.printClause(5,         0,              1,              0,                 0,                 1);
      cc.printClause(5,         0,              1,              0,                 1,                 0);
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

    for (unsigned i = 0; i < 29; i++) {
      // -18 50 -79 111 112 -113 0
      //                  c_out[0]       s_out[1]        s_out[2]        s_out[3]            a_in[3]            b_in[3]
      cc.setLiterals(6, start + i, output + 1 + i, output + 2 + i, output + 3 + i, inputs[0] + 3 + i, inputs[1] + 3 + i);
      cc.printClause(6,         0,              1,              1,              0,                 0,                 1);
      cc.printClause(6,         0,              1,              1,              0,                 1,                 0);
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
/*
// NEW START
    // 1,  33
    // 65
    // 96



// NEW ENDE
*/
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
