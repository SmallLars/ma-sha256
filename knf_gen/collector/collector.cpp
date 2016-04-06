#include "collector.h"

using std::vector;
using namespace CMSat;

Collector::Collector() {
}

Collector::~Collector() {
}

void Collector::newModul(unsigned level, const char* name, Modul* modul) {}

void Collector::create(bool xOR, const std::vector<CMSat::Lit>& vars) {}
