#include "sha256.h"

#include "add_prepare_32.h"
#include "shacore_ex1_32.h"
#include "clausecreator.h"

#include "../common/sha256tools.h"
#include "../common/solvertools.h"
#include "../common/general.h"

using std::vector;
using namespace CMSat;

unsigned Sha256::stats[STATS_LENGTH];

static uint32_t sha_k[64] = {\
  0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,\
  0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,\
  0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,\
  0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,\
  0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,\
  0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,\
  0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,\
  0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2};

// Information for additional clauses
static unsigned prepN[64];
static unsigned coreI[64][9];
static unsigned coreN[64];

static void clause_5_36(ClauseCreator &cc);
static void clause_4_39(ClauseCreator &cc);
static void clause_4_43(ClauseCreator &cc);
static void clause_4_45(ClauseCreator &cc);
static void clause_4_48(ClauseCreator &cc);

Sha256::Sha256() : Modul(32, 24, 8) {
  for (unsigned i = 0; i < 24; i++) {
    inputs.push_back(i * 32);
  }

  start = 768;
  output = 0;
}

Sha256::~Sha256() {
}

unsigned* Sha256::getStats() {
  return stats;
}

void Sha256::create(Printer* printer) {
  // Input
  unsigned global_input[64];
  for (unsigned i = 0; i < 16; i++) global_input[i] = inputs[i];

  // Status
  unsigned vars[8];
  for (unsigned i = 16; i < 24; i++) vars[i - 16] = inputs[i];

  // 512 Inputbits und 256 Statebits
  unsigned newvars = 0;

  Add_Prepare_32 adder;
  ShaCore_Ex1_32 core(0);

  for (unsigned i = 0; i < 64; i++) {
    vector<unsigned> subinputs;
    for (unsigned n = 0; n < 8; n++) {
      subinputs.push_back(vars[n]);
      coreI[i][n] = vars[n];
    }

    if (i < 16) {
      subinputs.push_back(global_input[i]);
    } else {
      vector<unsigned> prepareinputs;
      prepareinputs.push_back(global_input[i - 16]);
      prepareinputs.push_back(global_input[i - 15]);
      prepareinputs.push_back(global_input[i -  7]);
      prepareinputs.push_back(global_input[i -  2]);
      adder.setInputs(prepareinputs);
      adder.setStart(start + newvars);
      prepN[i] = start + newvars;
      adder.create(printer);
      newvars += adder.getAdditionalVarCount();

      global_input[i] = adder.getOutput();
      subinputs.push_back(adder.getOutput());
    }
    coreI[i][8] = global_input[i];

    core.setValue(sha_k[i]);
    core.setInputs(subinputs);
    core.setStart(start + newvars);
    coreN[i] = start + newvars;
    core.create(printer);
    newvars += core.getAdditionalVarCount();

    for (unsigned n = 7; n > 0; n--) vars[n] = vars[n - 1];
    vars[0] = core.getOutput();
    vars[4] = core.getOutput() + 32;
  }

#ifdef ADDITIONAL_CLAUSES
  ClauseCreator cc(printer);

  // distance - modulcount + 1 = 5
  clause_5_36(cc);

  // distance - modulcount + 1 = 4
  clause_4_39(cc);
  clause_4_43(cc);
  clause_4_45(cc);
  clause_4_48(cc);

  for (unsigned i = 0; i < 64; i++) {
    /*
    std::cout << (i < 16 ? 0 : prepN[i] + 1);
    std::cout << " " << coreI[i][0] + 1 << " " << coreI[i][1] + 1 << " " << coreI[i][2] + 1 << " " << coreI[i][3] + 1;
    std::cout << " " << coreI[i][4] + 1 << " " << coreI[i][5] + 1 << " " << coreI[i][6] + 1 << " " << coreI[i][7] + 1;
    std::cout << " " << coreN[i] + 1 << "\n";
    */

    // distance - modulcount + 1 = 3
/*
    if (i < 39) {
      if (in_array(i, 3, 0, 8, 23)) {
        // 773 -10035 -11775 17403 0
        cc.setLiterals(4, coreN[i] + 4, prepN[i + 16] + 162, coreN[i + 18] + 5, prepN[i + 25] + 132);
        cc.printClause(4,            1,                   0,                 0,                   1);
      }
      if (in_array(i, 5, 1, 3, 15, 29, 38)) {
        // 2500 -12490 -14260 19919 0
        cc.setLiterals(4, coreN[i] + 24, prepN[i + 16] + 151, coreN[i + 18] + 24, prepN[i + 25] + 182);
        cc.printClause(4,             1,                   0,                  0,                   1);
      }
      if (in_array(i, 11, 0, 6, 8, 12, 15, 19, 22, 26, 33, 34, 36)) {
        // 738 -1087 10001 -17399 0
        cc.setLiterals(4, coreI[i][7] + 1, coreN[i] + 318, prepN[i + 16] + 128, prepN[i + 25] + 128);
        cc.printClause(4,               1,              0,                   1,                   0);
      }
      if (in_array(i, 13, 1, 4, 8, 14, 15, 16, 18, 24, 27, 28, 29, 31, 32)) {
        // 8736 -21572 -23279 28907 0
        cc.setLiterals(4, coreN[i] + 1, prepN[i + 16] + 191, coreN[i + 18] + 1, prepN[i + 25] + 128);
        cc.printClause(4,            1,                   0,                 0,                   1);
      }
      if (in_array(i, 14, 2, 3, 8, 10, 12, 14, 15, 17, 20, 22, 24, 27, 29, 31)) {
        // 2478 -12468 -14238 19897 0
        cc.setLiterals(4, coreN[i] + 2, prepN[i + 16] + 129, coreN[i + 18] + 2, prepN[i + 25] + 160);
        cc.printClause(4,            1,                   0,                 0,                   1);
      }
      if (in_array(i, 18, 0, 1, 2, 6, 7, 8, 12, 15, 16, 19, 20, 22, 26, 32, 33, 34, 36, 38)) {
        // -1339 10824 12595 -18223 0
        cc.setLiterals(4, coreN[i] + 1, prepN[i + 16] + 129, coreN[i + 18] + 3, prepN[i + 25] + 130);
        cc.printClause(4,            0,                   1,                 1,                   0);
      }
      if (!in_array(i, 18, 1, 4, 7, 8, 14, 15, 16, 18, 20, 21, 24, 27, 28, 29, 31, 32, 35, 38)) {
        // -2508 12467 14237 -19865 0
        cc.setLiterals(4, coreN[i] + 32, prepN[i + 16] + 128, coreN[i + 18] + 1, prepN[i + 25] + 128);
        cc.printClause(4,             0,                   1,                 1,                   0);
      }
      if (!in_array(i, 17, 0, 4, 6, 9, 10, 14, 16, 17, 19, 21, 25, 26, 28, 31, 32, 33, 37)) {
        // 5333 -16588 -18358 23986 0
        cc.setLiterals(4, coreN[i] + 12, prepN[i + 16] + 139, coreN[i + 18] + 12, prepN[i + 25] + 139);
        cc.printClause(4,             1,                   0,                  0,                   1);
      }
      if (!in_array(i, 16, 3, 4, 5, 10, 14, 17, 18, 23, 24, 25, 27, 28, 29, 31, 35, 37)) {
        // -1339 10824 12594 -18222 0
        cc.setLiterals(4, coreN[i] + 1, prepN[i + 16] + 129, coreN[i + 18] + 2, prepN[i + 25] + 129);
        cc.printClause(4,            0,                   1,                 1,                   0);
      }
    }
*/
    if (i < 45) {
      if (in_array(i, 17, 2, 5, 8, 9, 12, 15, 16, 17, 19, 23, 25, 29, 31, 40, 42, 43, 44)) {
        // -3582 5639 10949 -16577 0
        cc.setLiterals(4, coreI[i][4], coreN[i + 3] + 318, coreN[i + 12] + 1, prepN[i + 19] + 128);
        cc.printClause(4,           0,                  1,                 1,                   0);
      }

      cc.setLiterals(4, coreI[i][4] + 1, coreN[i + 3] + 318, coreN[i + 12] + 1, prepN[i + 19] + 159);
      if (in_array(i, 8, 1, 8, 11, 15, 17, 25, 26, 40)) {
        // 1307 -3363 8167 -13320 0
        // 6997 -9053 15881 -21540 0
        // 18062 -21130 28211 -33870 0
        cc.printClause(4,             1,                  0,                 1,                   0);
      }
      if (in_array(i, 20, 0, 1, 3, 7, 10, 11, 14, 21, 22, 24, 26, 28, 30, 32, 33, 34, 35, 37, 38, 39)) {
        // 1307 -3363 -8167 13320 0
        // 6997 -9053 -15881 21540 0
        // 18062 -21130 -28211 33870 0
        cc.printClause(4,             1,                  0,                 0,                   1);
      }
      if (in_array(i, 21, 0, 3, 4, 6, 7, 10, 13, 14, 18, 20, 21, 22, 24, 27, 30, 33, 35, 36, 37, 38, 41)) {
        cc.printClause(4,             1,                  0,                 0,                   0);
      }
      if (in_array(i, 11, 2, 6, 16, 19, 23, 29, 31, 36, 41, 42, 43)) {
        cc.printClause(4,             1,                  0,                 1,                   1);
      }
    }
    if (i < 48) {
      if (in_array(i, 17, 0, 3, 6, 8, 12, 15, 19, 22, 25, 26, 33, 34, 36, 40, 42, 45, 47)) {
        // -2477 -7915 12467 0
        cc.setLiterals(3, coreN[i] + 1, coreN[i + 9] + 318, prepN[i + 16] + 128);
        cc.printClause(3,            0,                  0,                   1);
      }
      if (in_array(i, 15, 2, 4, 11, 14, 20, 21, 23, 27, 28, 29, 31, 35, 37, 41, 43)) {
        // 15881 -23596 -28907 0
        cc.setLiterals(3, coreN[i] + 1, coreN[i + 9] + 318, prepN[i + 16] + 128);
        cc.printClause(3,            1,                  0,                   0);
      }
      if (in_array(i, 6, 2, 10, 13, 18, 19, 34)) {
        // -680 2232 7036 -11652 0
        cc.setLiterals(4, coreI[i][7] + 7, coreN[i] + 325, coreN[i + 9] + 8, prepN[i + 16] + 135);
        cc.printClause(4,               0,              1,                1,                   0);
      }
      if (!in_array(i, 17, 2, 5, 7, 11, 16, 18, 20, 21, 23, 28, 30, 31, 32, 35, 37, 43, 46)) {
        // 738 -1087 -5890 10001 0
        cc.setLiterals(4, coreI[i][7] + 1, coreN[i] + 318, coreN[i + 9], prepN[i + 16] + 128);
        cc.printClause(4,               1,              0,            0,                   1);
      }
      if (in_array(i, 8, 0, 3, 6, 25, 33, 36, 40, 42)) {
        // 738 -1087 -5922 10001 0
        cc.setLiterals(4, coreI[i][7] + 1, coreN[i] + 318, coreN[i + 9] + 32, prepN[i + 16] + 128);
        cc.printClause(4,               1,              0,                 0,                   1);
      }
      if (in_array(i, 9, 0, 3, 4, 14, 18, 25, 29, 40, 42)) {
        // 738 -1087 5922 10032 0
        cc.setLiterals(4, coreI[i][7] + 1, coreN[i] + 318, coreN[i + 9] + 32, prepN[i + 16] + 159);
        cc.printClause(4,               1,              0,                 1,                   1);
        // 738 -1087 -5922 -10032 0
        cc.setLiterals(4, coreI[i][7] + 1, coreN[i] + 318, coreN[i + 9] + 32, prepN[i + 16] + 159);
        cc.printClause(4,               1,              0,                 0,                   0);
      }
      if (in_array(i, 7, 1, 4, 9, 14, 29, 39, 44)) {
        // 1338 -6777 -10823 10853 0
        cc.setLiterals(4, coreN[i], coreN[i + 9] + 318, prepN[i + 16] + 128, prepN[i + 16] + 158);
        cc.printClause(4,        1,                  0,                   0,                   1);
      }
      if (in_array(i, 18, 0, 1, 6, 8, 9, 12, 15, 19, 22, 26, 33, 34, 36, 39, 42, 44, 45, 47)) {
        // -1339 4721 6808 10823 0
        cc.setLiterals(4, coreN[i] + 1, coreI[i + 6][4] + 1, coreN[i + 9] + 349, prepN[i + 16] + 128);
        cc.printClause(4,            0,                   1,                  1,                   1);
      }
      if (!in_array(i, 21, 1, 2, 4, 5, 7, 9, 11, 14, 16, 20, 28, 29, 30, 31, 32, 35, 37, 39, 43, 44, 46)) {
        // -2476 5859 -7915 12467 0
        cc.setLiterals(4, coreN[i], coreI[i + 6][4] + 1, coreN[i + 9] + 318, prepN[i + 16] + 128);
        cc.printClause(4,        0,                   1,                  0,                   1);
      }
      if (in_array(i, 17, 0, 3, 6, 8, 12, 15, 19, 22, 25, 26, 33, 34, 36, 40, 42, 45, 47)) {
        // -2477 5859 -7946 12467 0
        cc.setLiterals(4, coreN[i] + 1, coreI[i + 6][4] + 1, coreN[i + 9] + 349, prepN[i + 16] + 128);
        cc.printClause(4,            0,                   1,                  0,                   1);
      }
      if (in_array(i, 17, 0, 3, 6, 8, 12, 15, 19, 22, 25, 26, 33, 34, 36, 40, 42, 45, 47)) {
        // -2477 -7915 -12497 -12498 0
        cc.setLiterals(4, coreN[i] + 1, coreN[i + 9] + 318, prepN[i + 16] + 158, prepN[i + 16] + 159);
        cc.printClause(4,            0,                  0,                   0,                   0);
      }
      if (in_array(i, 17, 0, 5, 6, 8, 12, 15, 19, 22, 26, 32, 33, 34, 36, 42, 45, 46, 47)) {
        // -23279 -30994 -31057 36336 0
        cc.setLiterals(4, coreN[i] + 1, coreN[i + 9] + 318, coreN[i + 9] + 381, prepN[i + 16] + 159);
        cc.printClause(4,            0,                  0,                  0,                   1);
      }
    }
  }
#endif
/*
  ClauseCreator cc(printer);
  for (unsigned i = 0; i < 64; i++) {
    if (i < 39) {
      // 0
      for (unsigned b = 0; b < 30; b++) {
        // 773 -10035 -11775 17403 0
        //                carry[0]                result[0]               carry[1]                 carry[1]
        cc.setLiterals(4, coreN[i] + b, prepN[i + 16] + 158 + b, coreN[i + 18] + 1 + b, prepN[i + 25] + 128 + b);
        cc.printClause(4,            1,                       0,                     0,                       1);
      }

      // 1
      for (unsigned b = 0; b < 31; b++) {
        // 2478 -12468 -14238 19897 0
        // 2500 -12490 -14260 19919 0
        //                carry[0]                 carry[0]           carry[0]                result[0]
        cc.setLiterals(4, coreN[i] + b, prepN[i + 16] + 127 + b, coreN[i + 18] + b, prepN[i + 25] + 158 + b);
        cc.printClause(4,            1,                       0,                 0,                       1);
      }

      // 2
      for (unsigned b = 0; b < 31; b++) {
        // 738 -1087 10001 -17399 0
        //                  result[0]            carry[0]                 carry[0]                 carry[0]
        cc.setLiterals(4, coreI[i][7] + b, coreN[i] + 317 + b, prepN[i + 16] + 127 + b, prepN[i + 25] + 127 + b);
        cc.printClause(4,               1,                  0,                       1,                       0);
      }

      // 3
      for (unsigned b = 0; b < 31; b++) {
        // 8736 -21572 -23279 28907 0
        //                carry[0]                 carry[0]           carry[0]                 carry[0]
        cc.setLiterals(4, coreN[i] + b, prepN[i + 16] + 190 + b, coreN[i + 18] + b, prepN[i + 25] + 127 + b);
        cc.printClause(4,            1,                       0,                 0,                       1);
      }

      // 4
      for (unsigned b = 0; b < 29; b++) {
        // -1339 10824 12595 -18223 0
        //                carry[0]                 carry[1]               carry[2]                 carry[2]
        cc.setLiterals(4, coreN[i] + b, prepN[i + 16] + 128 + b, coreN[i + 18] + 2 + b, prepN[i + 25] + 129 + b);
        cc.printClause(4,            0,                       1,                     1,                       0);
      }

      // 5
      for (unsigned b = 0; b < 31; b++) {
        // -2508 12467 14237 -19865 0
        //                    result[0]                 carry[0]           carry[0]                 carry[0]
        cc.setLiterals(4, coreN[i] + 31 + b, prepN[i + 16] + 127 + b, coreN[i + 18] + b, prepN[i + 25] + 127 + b);
        cc.printClause(4,                 0,                       1,                 1,                       0);
      }

      // 6
      for (unsigned b = 0; b < 31; b++) {
        // 5333 -16588 -18358 23986 0
        //                carry[0]                 carry[0]           carry[0]                 carry[0]
        cc.setLiterals(4, coreN[i] + b, prepN[i + 16] + 127 + b, coreN[i + 18] + b, prepN[i + 25] + 127 + b);
        cc.printClause(4,            1,                       0,                 0,                       1);
      }

      // 7
      for (unsigned b = 0; b < 30; b++) {
        // -1339 10824 12594 -18222 0
        //                carry[0]                 carry[1]               carry[1]                 carry[1]
        cc.setLiterals(4, coreN[i] + b, prepN[i + 16] + 128 + b, coreN[i + 18] + 1 + b, prepN[i + 25] + 128 + b);
        cc.printClause(4,            0,                       1,                     1,                       0);
      }
    }
  }
*/
}

MU_TEST_C(Sha256::test) {
  SolverConf config;
  config.verbosity = 0;//9;

  SATSolver solver(config);
  solver.log_to_file("test.log");

  uint32_t input[16];
  sha256_padding(input, "Das ist eine Eingabe aus der ein Hash erstellt wird.");
  for (unsigned i = 0; i < 16; i++) {
    solver_writeInt(solver, i * 32, 32, input[i]);
  }

  uint32_t state[8] = {0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19};
  for (unsigned i = 16; i < 24; i++) {
    solver_writeInt(solver, i * 32, 32, state[i - 16]);
  }

  Sha256 sha256;
  sha256.append(&solver);

  lbool ret = solver.solve();
  mu_assert(ret == l_True, "SHA256 UNSAT");

  uint32_t output[8] = {0x27931f0e, 0x7e53670d, 0xdbec1a1c, 0xe23e21b4, 0x663c63c0, 0xd17117ee, 0x1a934bc0, 0xc294dbe9};
  uint32_t out_lsb[8] = {49264, 48442, 47620, 46798, 49296, 48474, 47652, 46830};

  for (unsigned i = 0; i < 8; i++) {
    mu_assert(output[i] - state[i] == solver_readInt(solver, out_lsb[i], 32), "SHA256 failed");
  }
}

static void clause_5_36(ClauseCreator &cc) {
/*
    distance = 8
    modulcount = 4

    For every bit from 0 to 28 do:
        If round const bit 2 is zero:
            While zero bits follow:
                Clause with 4 literals is valid
            On first one bit clause with 5 literals is valid
        From 0 or after first one bit:
            Clause with 5 literals is valid if const bit is zero

    valid for r < 36
*/
  for (unsigned r = 0; r < 36; r++) {
    unsigned b = 0;
    if ((sha_k[r + 21] & 0x2) == 0) {
      for (; true ; b++) {
        if ((sha_k[r + 21] >> (b + 2)) & 0x1) {
          goto start;
        } else {
          // -15595 -31374 -33144 38772 0
          //                  result[0]                 carry[2]               carry[2]                 carry[2]
          cc.setLiterals(4, coreI[r][4] + b, prepN[r + 19] + 129 + b, coreN[r + 21] + 2 + b, prepN[r + 28] + 129 + b);
          cc.printClause(4,               0,                       0,                     0,                       1);
        }
      }
    }
    for (; b < 29; b++) {
      if (((sha_k[r + 21] >> (b + 2)) & 0x1) == 0) {
        start:
        // -1875 -14112 -14143 -15882 21510 0
        //                  result[0]                 carry[2]                result[2]               carry[2]                 carry[2]
        cc.setLiterals(5, coreI[r][4] + b, prepN[r + 19] + 129 + b, prepN[r + 19] + 160 + b, coreN[r + 21] + 2 + b, prepN[r + 28] + 129 + b);
        cc.printClause(5,               0,                       0,                       0,                     0,                       1);
      }
    }
  }
}

static void clause_4_39(ClauseCreator &cc) {
/*
    distance = 8
    modulcount = 5

    Clause with 4 literals is valid on bit -2
    While zero bits follow:
        Clause with 4 literals is valid
    If bit > 0:
        Clause with 5 literals is valid
    For every bit from max(0, bit) to 28 do:
         Clause with 5 literals is valid if const bit is zero

    valid for r < 39
*/
  for (unsigned r = 0; r < 39; r++) {
    signed b = -2;
    for (; true ; b++) {
      // 7916 -19866 -21636 27264 0
      // 15377 -28086 -29856 35484 0
      //                      carry[2]                 carry[2]               carry[2]                 carry[2]
      cc.setLiterals(4, coreN[r] + 319 + b, prepN[r + 16] + 129 + b, coreN[r + 18] + 2 + b, prepN[r + 25] + 129 + b);
      cc.printClause(4,                  1,                       0,                     0,                       1);
      if ((sha_k[r + 18] >> (b + 3)) & 0x1) {
        b++;
        goto start;
      }
    }
    for (; b < 29; b++) {
      // Exceptions
      if (r ==  0 && b ==  0) goto start;
      if (r == 10 && b ==  2) goto start;
      if (r == 11 && b == 10) goto start;
      if (r == 13 && b ==  3) goto start;
      if (r == 15 && b == 14) goto start;
      if (r == 17 && b == 17) goto start;
      if (r == 27 && b ==  7) goto start;
      if (r == 28 && b ==  5) goto start;

      if (((sha_k[r + 18] >> (b + 2)) & 0x1) == 0) {
        start:
        if (b < 0) continue;
        // -737 1088 -10002 -10033 -11772 17400 0
        //                  result[0]            carry[2]                 carry[2]                result[2]               carry[2]                 carry[2]
        cc.setLiterals(6, coreI[r][7] + b, coreN[r] + 319 + b, prepN[r + 16] + 129 + b, prepN[r + 16] + 160 + b, coreN[r + 18] + 2 + b, prepN[r + 25] + 129 + b);
        cc.printClause(6,               0,                  1,                       0,                       0,                     0,                       1);
      }
    }
  }
}

static void clause_4_43(ClauseCreator &cc) {
  for (unsigned r = 0; r < 43; r++) {
    if (in_array(r, 7, 18, 20, 24, 28, 32, 34, 38)) {
      unsigned carry_0 = (r == 24 || r == 38 ? CC_DC : 1);

      // -11486 14237 -21952 22014 -22015 27294 0
      // -13130 15881 -23596 23658 -23659 28938 0
      //                      result[1]          carry[1]             carry[1]             carry[0]             carry[1]            result[1]
      cc.setLiterals(6, coreI[r][4] + 1, coreN[r + 3] + 1, coreN[r + 12] + 318, coreN[r + 12] + 380, coreN[r + 12] + 381, prepN[r + 19] + 159);
      cc.printClause(6,               0,                1,                   0,             carry_0,                   0,                   1);

      //                  result[0]          carry[1]             carry[1]             carry[0]             carry[1]            result[1]
      cc.setLiterals(6, coreI[r][4], coreN[r + 3] + 1, coreN[r + 12] + 318, coreN[r + 12] + 380, coreN[r + 12] + 381, prepN[r + 19] + 159);
      cc.printClause(6,           0,                1,                   0,             carry_0,                   0,                   1);
    }
    if (r == 28) {
      //                      result[2]          carry[2]             carry[2]             carry[1]             carry[2]            result[2]
      cc.setLiterals(6, coreI[r][4] + 2, coreN[r + 3] + 2, coreN[r + 12] + 319, coreN[r + 12] + 381, coreN[r + 12] + 382, prepN[r + 19] + 160);
      cc.printClause(6,               0,                1,                   0,                   1,                   0,                   1);

      //                  result[1]          carry[2]             carry[2]             carry[1]             carry[2]            result[2]
      cc.setLiterals(6, coreI[r][4] + 1, coreN[r + 3] + 2, coreN[r + 12] + 319, coreN[r + 12] + 381, coreN[r + 12] + 382, prepN[r + 19] + 160);
      cc.printClause(6,               0,                1,                   0,                   1,                   0,                   1);
    }
    if (in_array(r, 8, 0, 1, 11, 15, 22, 26, 37, 39)) {
      // -14774 17556 -25240 25302 -25303 30582 0
      // -27104 29886 -37570 37632 -37633 42912 0
      //                      result[1]          result[1]             carry[1]             carry[0]             carry[1]            result[1]
      cc.setLiterals(6, coreI[r][4] + 1, coreN[r + 3] + 32, coreN[r + 12] + 318, coreN[r + 12] + 380, coreN[r + 12] + 381, prepN[r + 19] + 159);
      cc.printClause(6,               0,                 1,                   0,                   1,                   0,                   1);

      // -18061 20844 -28528 28590 -28591 33870 0
      //                  result[0]          result[1]             carry[1]             carry[0]             carry[1]            result[1]
      cc.setLiterals(6, coreI[r][4], coreN[r + 3] + 32, coreN[r + 12] + 318, coreN[r + 12] + 380, coreN[r + 12] + 381, prepN[r + 19] + 159);
      cc.printClause(6,           0,                 1,                   0,                   1,                   0,                   1);
    }
  }
}

static void clause_4_45(ClauseCreator &cc) {
  for (unsigned r = 0; r < 45; r++) {
    if (in_array(r, 7, 18, 20, 24, 28, 32, 34, 38)) {
      // -11485 14237 -21952 22014 -22015 27294 0
      // -13129 15881 -23596 23658 -23659 28938 0
      //                  result[0]          carry[1]             carry[1]             carry[0]             carry[1]            result[1]
      cc.setLiterals(6, coreI[r][4], coreN[r + 3] + 1, coreN[r + 12] + 318, coreN[r + 12] + 380, coreN[r + 12] + 381, prepN[r + 19] + 159);
      cc.printClause(6,           0,                1,                   0,                   1,                   0,                   1);
    }
    if (r == 28) {
      //                   result[1]       carry[2]         carry[2]         carry[1]         carry[2]        result[2]
      cc.setLiterals(6, coreI[28][4], coreN[31] + 1, coreN[40] + 318, coreN[40] + 380, coreN[40] + 381, prepN[47] + 159);
      cc.printClause(6,            0,             1,               0,               1,               0,               1);
    }
  }
}

static void clause_4_48(ClauseCreator &cc) {
  for (unsigned r = 0; r < 48; r++) {
    // 1
    if (in_array(r, 10, 0, 7, 16, 21, 23, 30, 31, 35, 37, 42)) {
      // -738 2 -6208 6270 -6271 10032 0
      // -28748 31467 -39214 39276 -39277 44556 0
      //                      result[1]        result[1]            carry[1]            carry[0]            carry[1]            result[1]
      cc.setLiterals(6, coreI[r][7] + 1, coreI[r][8] + 1, coreN[r + 9] + 318, coreN[r + 9] + 380, coreN[r + 9] + 381, prepN[r + 16] + 159);
      cc.printClause(6,               0,               1,                  0,                  1,                  0,                   1);

      // -737 2 -6208 6270 -6271 10032 0
      // -28747 31467 -39214 39276 -39277 44556 0
      //                  result[0]        result[1]            carry[1]            carry[0]            carry[1]            result[1]
      cc.setLiterals(6, coreI[r][7], coreI[r][8] + 1, coreN[r + 9] + 318, coreN[r + 9] + 380, coreN[r + 9] + 381, prepN[r + 16] + 159);
      cc.printClause(6,           0,               1,                  0,                  1,                  0,                   1);

      // -737 -738 2 -6208 6270 -6271 10032 0
      // -28747 -28748 31467 -39214 39276 -39277 44556 0
    }
    // 2.0
    if (in_array(r, 21, 0, 3, 5, 6, 8, 12, 15, 18, 19, 22, 25, 26, 32, 33, 34, 36, 40, 42, 45, 46, 47)) {
      // -9273 -11771 -19486 19548 -19549 24828 0
      //                      result[1]      carry[1]            carry[1]            carry[0]            carry[1]            result[1]
      cc.setLiterals(6, coreI[r][7] + 1, coreN[r] + 1, coreN[r + 9] + 318, coreN[r + 9] + 380, coreN[r + 9] + 381, prepN[r + 16] + 159);
      cc.printClause(6,               0,            0,                  0,                  1,                  0,                   1);

      // -9272 -11771 -19486 19548 -19549 24828 0
      //                  result[0]      carry[1]            carry[1]            carry[0]            carry[1]            result[1]
      cc.setLiterals(6, coreI[r][7], coreN[r] + 1, coreN[r + 9] + 318, coreN[r + 9] + 380, coreN[r + 9] + 381, prepN[r + 16] + 159);
      cc.printClause(6,           0,            0,                  0,                  1,                  0,                   1);

      // -9272 -9273 -11771 -19486 19548 -19549 24828 0
    }
    // 2.1
    if (in_array(r, 8, 0, 5, 8, 26, 33, 40, 42, 47)) {
      //                      result[2]      carry[2]            carry[2]            carry[1]            carry[2]            result[2]
      cc.setLiterals(6, coreI[r][7] + 2, coreN[r] + 2, coreN[r + 9] + 319, coreN[r + 9] + 381, coreN[r + 9] + 382, prepN[r + 16] + 160);
      cc.printClause(6,               0,            0,                  0,                  1,                  0,                   1);

      //                      result[1]      carry[2]            carry[2]            carry[1]            carry[2]            result[2]
      cc.setLiterals(6, coreI[r][7] + 1, coreN[r] + 2, coreN[r + 9] + 319, coreN[r + 9] + 381, coreN[r + 9] + 382, prepN[r + 16] + 160);
      cc.printClause(6,               0,            0,                  0,                  1,                  0,                   1);
    }
    // 2.2
    if (in_array(r, 2, 42, 47)) {
      //                      result[3]      carry[3]            carry[3]            carry[2]            carry[3]            result[3]
      cc.setLiterals(6, coreI[r][7] + 3, coreN[r] + 3, coreN[r + 9] + 320, coreN[r + 9] + 382, coreN[r + 9] + 383, prepN[r + 16] + 161);
      cc.printClause(6,               0,            0,                  0,                  1,                  0,                   1);

      //                      result[2]      carry[3]            carry[3]            carry[2]            carry[3]            result[3]
      cc.setLiterals(6, coreI[r][7] + 2, coreN[r] + 3, coreN[r + 9] + 320, coreN[r + 9] + 382, coreN[r + 9] + 383, prepN[r + 16] + 161);
      cc.printClause(6,               0,            0,                  0,                  1,                  0,                   1);
    }
    // 3
    if (in_array(r, 9, 0, 3, 4, 14, 18, 25, 29, 40, 42)) {
      // -642 2508 -7915 7977 -7978 12498 0
      //                      result[1]      result[1]            carry[1]            carry[0]            carry[1]            result[1]
      cc.setLiterals(6, coreI[r][7] + 1, coreN[r] + 32, coreN[r + 9] + 318, coreN[r + 9] + 380, coreN[r + 9] + 381, prepN[r + 16] + 159);
      cc.printClause(6,               0,             1,                  0,                  1,                  0,                   1);

      // -641 2508 -7915 7977 -7978 12498 0
      //                  result[0]      result[1]            carry[1]            carry[0]            carry[1]            result[1]
      cc.setLiterals(6, coreI[r][7], coreN[r] + 32, coreN[r + 9] + 318, coreN[r + 9] + 380, coreN[r + 9] + 381, prepN[r + 16] + 159);
      cc.printClause(6,           0,             1,                  0,                  1,                  0,                   1);

      // -641 -642 2508 -7915 7977 -7978 12498 0
    }
  }
}
