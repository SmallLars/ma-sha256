#include "moduldb.h"

#include <algorithm>

#include "../module/modul.h"

using std::vector;
using std::pair;
using std::ostream;
using namespace CMSat;

ModulDB::ModulDB() {
}

ModulDB::~ModulDB() {
/*
    for (unsigned m = 0; m < module.size(); m++) {
        module[m].print(std::cout);
    }
*/
}

void ModulDB::newModul(unsigned level, const char* name, Modul* modul) {
    if (level < 10) return;

    ModulEntry newModul;
    newModul.level = level;
    newModul.name = name;

    // Inputs
    for (unsigned i = 0; i < modul->getInputs().size(); i++) {
        if (i > 0 && newModul.ranges.back().first + newModul.ranges.back().second == modul->getInputs()[i]) {
            newModul.ranges.back().second += modul->getBitWidth();
            continue;
        }
        newModul.ranges.push_back(pair<unsigned, unsigned>(modul->getInputs()[i], modul->getBitWidth()));
    }

    // Additional Vars
    if (newModul.ranges.size() > 0 && newModul.ranges.back().first + newModul.ranges.back().second == modul->getStart()) {
        newModul.ranges.back().second += (modul->getAdditionalVarCount() - modul->getOutputNum());
    } else {
        newModul.ranges.push_back(pair<unsigned, unsigned>(modul->getStart(), (modul->getAdditionalVarCount() - modul->getOutputNum())));
    }

    // Output
    if (newModul.ranges.size() > 0 && newModul.ranges.back().first + newModul.ranges.back().second == modul->getOutput()) {
        newModul.ranges.back().second += modul->getOutputNum();
    } else {
        newModul.ranges.push_back(pair<unsigned, unsigned>(modul->getOutput(), modul->getOutputNum()));
    }

    for (vector<ModulEntry>::iterator it = module.begin(); it < module.end(); it++) {
        if ((*it).level > level) {
            module.insert(it, newModul);
            return;
        }
    }
    module.push_back(newModul);
}

ModulEntry* ModulDB::isInSingleModul(vector<Lit>& clause) {
    for (unsigned m = 0; m < module.size(); m++) {
        bool isInside;
        for (unsigned lit = 0; lit < clause.size(); lit++) {
            isInside = false;
            for (unsigned range = 0; range < module[m].ranges.size(); range++) {
                if (clause[lit].var() >= module[m].ranges[range].first && clause[lit].var() < module[m].ranges[range].first + module[m].ranges[range].second) {
                    isInside = true;
                    break;
                }
            }
            if (!isInside) break;
        }
        if (isInside) return &module[m];
    }
    return NULL;
}

void ModulEntry::print(ostream& out) {
    out << "(" << level << ") " << name << ":";
    for (unsigned range = 0; range < ranges.size(); range++) {
        out << " " << ranges[range].first + 1 << ":" << ranges[range].second;
    }
    out << "\n";
}
