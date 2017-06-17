#include "prepare_32.h"

#include "add_32.h"
#include "add_ssig_32.h"

#include "../common/solvertools.h"

using std::vector;
using namespace CMSat;

unsigned Prepare_32::stats[STATS_LENGTH];

Prepare_32::Prepare_32() : Modul(32, 4, 1) {
    Add_32 adder;
    Add_Ssig_32 adderSsig;
    output = start + adderSsig.getAdditionalVarCount() + 2 * adder.getAdditionalVarCount() - 32;
}

Prepare_32::~Prepare_32() {
}

unsigned* Prepare_32::getStats() {
    return stats;
}

void Prepare_32::create(Collector* collector) {
    collector->newModul(12, "Prepare_32", this);

    unsigned newvars = 0;
    vector<unsigned> subinputs;

    subinputs.clear();
    subinputs.push_back(inputs[1]);
    subinputs.push_back(inputs[3]);
    Add_Ssig_32 adder1;
    adder1.setInputs(subinputs);
    adder1.setStart(start + newvars);
    adder1.create(collector);
    newvars += adder1.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(inputs[0]);
    subinputs.push_back(inputs[2]);
    Add_32 adder2;
    adder2.setInputs(subinputs);
    adder2.setStart(start + newvars);
    adder2.create(collector);
    newvars += adder2.getAdditionalVarCount();

    subinputs.clear();
    subinputs.push_back(adder1.getOutput());
    subinputs.push_back(adder2.getOutput());
    Add_32 adder3;
    adder3.setInputs(subinputs);
    adder3.setStart(start + newvars);
    adder3.create(collector);
    newvars += adder3.getAdditionalVarCount();

#ifdef ADDITIONAL_CLAUSES
    ClauseCreator cc(collector);

    mirror_block(cc, inputs[0],  inputs[2],  95, 127);
    mirror_block(cc,     start, start + 32, 158,  64);
#endif
}

MU_TEST_C(Prepare_32::test) {
    unsigned a[] = {0xabcdef98};
    unsigned b[] = {0x651d8fa1};
    unsigned c[] = {0x456af012};
    unsigned d[] = {0x007fa4c5};

    for (unsigned t = 0; t < 1; t++) {
        SATSolver solver;
        solver.set_verbosity(0);
        solver.log_to_file("test.log");

        uint32_t s0 = (b[t] >> 7 | b[t] << (32-7)) ^ (b[t] >> 18 | b[t] << (32-18)) ^ (b[t] >> 3);
        uint32_t s1 = (d[t] >> 17 | d[t] << (32-17)) ^ (d[t] >> 19 | d[t] << (32-19)) ^ (d[t] >> 10);
        uint32_t ausgabe = a[t] + s0 + c[t] + s1;

        solver_writeInt(solver,  0, 32, a[t]);
        solver_writeInt(solver, 32, 32, b[t]);
        solver_writeInt(solver, 64, 32, c[t]);
        solver_writeInt(solver, 96, 32, d[t]);

        Prepare_32 adderPrepare;
        adderPrepare.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "ADDER_PREPARE UNSAT");
        mu_assert(ausgabe == solver_readInt(solver, adderPrepare.getOutput(), 32), "ADDER_PREPARE failed");
    }
}

//                                                            1,         65,        224,        256
//                                                          129,        161,        287,        193
void Prepare_32::mirror_block(ClauseCreator &cc, unsigned i, unsigned j, unsigned x, unsigned y) {
    // inputs[1]     inputs[3]     inputs[0]     inputs[2]
    //  33 -  64      97 - 128       1 -  32      65 -  96
    //     |             |             |             |
    //   ssig0         ssig1           |             |
    //   start      start +  32
    // 129 - 160     161 - 192         |             |
    //     |            |              |             |
    //           add                         add
    //       start +  64                 start + 127
    //        193 - 223                   256 - 286
    //       start +  95                 start + 158
    //        224 - 255                   287 - 318
    //            |                           |
    //                         add
    //                     start + 190
    //                      319 - 349
    //                       output
    //                      350 - 381

    for (unsigned b = 1; b < 2; b++) {
        //XXX b == 0 is useless
        //                 1        65       224         256                  319         350
        //               129       161       287         193                  319         350
        //         result[0] result[0] result[0]    carry[0]             carry[0]   result[0]
        cc.setLiterals(6, i + b, j + b, start + x + b, start + y + b, start + 190 + b, output + b);
        cc.printClause(6, CC_DC, CC_DC,         CC_DC,             0,               0,          0);
        cc.printClause(6,     0,     0,         CC_DC,         CC_DC,               0,          0);
        cc.printClause(6,     0,     0,             1,         CC_DC,               0,      CC_DC);
        cc.printClause(6,      1,CC_DC,         CC_DC,             0,               0,      CC_DC);
        cc.printClause(6,  CC_DC,    1,         CC_DC,             0,               0,      CC_DC);
        cc.printClause(6,  CC_DC,CC_DC,             1,             0,               0,      CC_DC);
    }

    //                       2        65        225            257              320         351
    //                     130       161        288            194              320         351
    //               result[1] result[0]  result[1]       carry[1]         carry[1]   result[1]
    cc.setLiterals(6,    i + 1,        j, start + 1 + x, start + 1 + y, start + 191, output + 1);
    //XXX cc.printClause(6,        1,        1,         CC_DC,             0,           0,      CC_DC);
    //XXX cc.printClause(6,        1,        0,         CC_DC,             0,           0,      CC_DC);
    cc.printClause(6,        1,    CC_DC,             1,             0,       CC_DC,          0);

    //                66       224            225            257              320         351
    //               162       287            288            194              320         351
    //         result[1] result[0]      result[1]       carry[1]         carry[1]   result[1]
    cc.setLiterals(6, j + 1, start + x, start + 1 + x, start + 1 + y, start + 191, output + 1);
    cc.printClause(6,     1,     CC_DC,             1,             0,       CC_DC,          0);
    //XXX cc.printClause(6,     1,         0,         CC_DC,             0,           0,      CC_DC);

    //                   67        225            258              321         351
    //                  163        288            195              321         351
    //            result[2]  result[1]       carry[2]         carry[2]   result[1]
    cc.setLiterals(5, j + 2, start + 1 + x, start + 2 + y, start + 192, output + 1);
    cc.printClause(5,     1,             1,             0,           0,      CC_DC);
    cc.printClause(5,     1,         CC_DC,             0,           0,          0);

    //                66        67        226            258              321         352
    //               162       163        289            195              321         352
    //         result[1] result[2]  result[2]       carry[2]         carry[2]   result[2]
    cc.setLiterals(6, j + 1, j + 2, start + 2 + x, start + 2 + y, start + 192, output + 2);
    cc.printClause(6,     1,     1,         CC_DC,             0,           0,      CC_DC);
    cc.printClause(6,     1, CC_DC,             1,             0,           0,      CC_DC);
    cc.printClause(6,     1, CC_DC,         CC_DC,             0,           0,          0);

    //                 2        66        225            256              319          320         351
    //               130       162        288            193              319          320         351
    //         result[1] result[1]  result[1]       carry[0]         carry[0]     carry[1]   result[1]
    cc.setLiterals(7, i + 1, j + 1, start + 1 + x, start + 0 + y, start + 190, start + 191, output + 1);
    cc.printClause(7,     1,     1,             1,         CC_DC,       CC_DC,           0,      CC_DC);
    cc.printClause(7,     1,     0,         CC_DC,             0,       CC_DC,           0,      CC_DC);
    cc.printClause(7,     0,     1,         CC_DC,             0,       CC_DC,           0,      CC_DC);
    cc.printClause(7,     1,     0,         CC_DC,         CC_DC,           0,           1,      CC_DC);
    cc.printClause(7,     0,     1,         CC_DC,         CC_DC,           0,           1,      CC_DC);
    cc.printClause(7,     1,     1,         CC_DC,         CC_DC,       CC_DC,           0,          0);

    //                 2        67        226            258              321         352
    //               130       163        289            195              321         352
    //         result[1] result[2]  result[2]       carry[2]         carry[2]   result[2]
    cc.setLiterals(6, i + 1, j + 2, start + 2 + x, start + 2 + y, start + 192, output + 2);
    cc.printClause(6,     1,     1,         CC_DC,             0,           0,      CC_DC);
    cc.printClause(6,     1, CC_DC,             1,             0,           0,      CC_DC);
    cc.printClause(6,     1, CC_DC,         CC_DC,             0,           0,          0);

    //                 3        66        225            258              321         351
    //               131       162        288            195              321         351
    //         result[2] result[1]  result[1]       carry[2]         carry[2]   result[1]
    cc.setLiterals(6, i + 2, j + 1, start + 1 + x, start + 2 + y, start + 192, output + 1);
    cc.printClause(6,     1,     1,         CC_DC,             0,           0,      CC_DC);
    cc.printClause(6,     1, CC_DC,             1,             0,           0,      CC_DC);
    cc.printClause(6,     1, CC_DC,         CC_DC,             0,           0,          0);

    //                 3        66        226            258              321         352
    //               131       162        289            195              321         352
    //         result[2] result[1]  result[2]       carry[2]         carry[2]   result[2]
    cc.setLiterals(6, i + 2, j + 1, start + 2 + x, start + 2 + y, start + 192, output + 2);
    cc.printClause(6,     1, CC_DC,             1,             0,           0,      CC_DC);
    cc.printClause(6,     1, CC_DC,         CC_DC,             0,           0,          0);

    //                      225        256            257              319         351
    //                      288        193            194              319         351
    //                result[1]   carry[0]       carry[1]         carry[0]   result[1]
    cc.setLiterals(5, start + 1 + x, start + y, start + 1 + y, start + 190, output + 1);
    cc.printClause(5,             1,         0,             1,       CC_DC,          1);
    cc.printClause(5,             0,         0,             1,       CC_DC,          0);
    cc.printClause(5,             0,     CC_DC,             0,           0,          0);
    cc.printClause(5,         CC_DC,         0,         CC_DC,           0,      CC_DC);

    //                      225            226            258              321         351         352
    //                      288            289            195              321         351         352
    //                result[1]      result[2]       carry[2]         carry[2]   result[1]   result[2]
    cc.setLiterals(6, start + 1 + x, start + 2 + x, start + 2 + y, start + 192, output + 1, output + 2);
    cc.printClause(6,             1,         CC_DC,             0,           0,      CC_DC,          0);
    cc.printClause(6,             1,             1,             0,           0,      CC_DC,      CC_DC);
    cc.printClause(6,         CC_DC,             1,             0,           0,          0,      CC_DC);
    cc.printClause(6,         CC_DC,         CC_DC,             0,           0,          0,          0);

    //                      225            257              319          321         351         352
    //                      288            194              319          321         351         352
    //                result[1]       carry[1]         carry[0]     carry[2]   result[1]   result[2]
    cc.setLiterals(6, start + 1 + x, start + 1 + y, start + 190, start + 192, output + 1, output + 2);
    cc.printClause(6,             1,             0,           0,       CC_DC,          1,      CC_DC);
    cc.printClause(6,         CC_DC,             0,       CC_DC,           0,          0,          0);

    //                      194             256            257              320
    //                      257             193            194              320
    //                 carry[1]        carry[0]       carry[1]         carry[1]
    cc.setLiterals(4, start + 1 + x - 31, start + y, start + 1 + y, start + 191);
    cc.printClause(4,                  0,         1,             0,           0);

    //                 2         3        258              321
    //               130       131        195              321
    //         result[1] result[2]   carry[2]         carry[2]
    cc.setLiterals(4, i + 1, i + 2, start + 2 + y, start + 192);
    cc.printClause(4,     1,     1,             0,           0);
}
