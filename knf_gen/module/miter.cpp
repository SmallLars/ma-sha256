#include "miter.h"

#include "sha256.h"

#include "clausecreator.h"

//#include <iomanip>

using std::vector;
using namespace CMSat;

unsigned Miter::stats[STATS_LENGTH];

Miter::Miter() : Modul(32, 40, 8) {
  Sha256 sha256;
  output = start + 2 * sha256.getAdditionalVarCount();
}

Miter::~Miter() {
}

unsigned* Miter::getStats() {
  return stats;
}

void Miter::create(Collector* collector) {
//  collector->newModul(40, "Miter", this);

  Sha256 sha256;

  vector<unsigned> subinputs;
  for (unsigned i =  0; i < 16; i++) subinputs.push_back(inputs[i]);
  for (unsigned i = 32; i < 40; i++) subinputs.push_back(inputs[i]);
  sha256.setInputs(subinputs);
  sha256.setStart(start);
  sha256.setOutput(output);
  sha256.create(collector);
/*
  for (vector<unsigned>::iterator it = sha256.getInputs().begin(); it != sha256.getInputs().end(); ++it) {
    std::cout << std::setw(4) << *it << " ";
  }
  std::cout << sha256.getStart() << " ";
  std::cout << sha256.getOutput() << "\n";
*/
  unsigned newvars = sha256.getAdditionalVarCount() - 256;

  subinputs.clear();
  for (unsigned i = 16; i < 40; i++) subinputs.push_back(inputs[i]);
  sha256.setInputs(subinputs);
  sha256.setStart(start + newvars);
  sha256.setOutput(output);
  sha256.create(collector);

  newvars += sha256.getAdditionalVarCount() - 256;
/*
  for (vector<unsigned>::iterator it = sha256.getInputs().begin(); it != sha256.getInputs().end(); ++it) {
    std::cout << std::setw(4) << *it << " ";
  }
  std::cout << sha256.getStart() << " ";
  std::cout << sha256.getOutput() << "\n";
*/

  vector<Lit> bigOrClause;
  for (unsigned i = 0; i < 16; i++) {
    for (unsigned b = 0; b < 32; b++) {
      createXOR(collector, start + newvars, inputs[i] + b, inputs[i + 16] + b);
      bigOrClause.push_back(Lit(start + newvars, false));
      newvars++;
    }
  }
  collector->create(false, bigOrClause);
}

MU_TEST_C(Miter::test) {
}
