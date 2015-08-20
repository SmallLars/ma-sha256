#include "moduldb.h"

#include "../module/modul.h"

using std::vector;
using std::pair;
using namespace CMSat;

struct ModulEntry {
    unsigned level;
    const char* name;
    vector< pair<unsigned, unsigned> > ranges;
};

ModulDB::ModulDB() {
}

ModulDB::~ModulDB() {
/*
    for (unsigned m = 0; m < module.size(); m++) {
        std::cout << "( " << module[m].level << ") " << module[m].name << ":";
        for (unsigned range = 0; range < module[m].ranges.size(); range++) {
            std::cout << " " << module[m].ranges[range].first << ":" << module[m].ranges[range].second;
        }
        std::cout << "\n";
    }
*/
}

void ModulDB::newModul(unsigned level, const char* name, Modul* modul) {
    if (level < 10) return;

    ModulEntry newModul;
    newModul.level = level;
    newModul.name = name;

    for (unsigned i = 0; i < modul->getInputs().size(); i++) {
        if (i > 0 && newModul.ranges.back().first + newModul.ranges.back().second == modul->getInputs()[i]) {
            newModul.ranges.back().second += modul->getBitWidth();
            continue;
        }
        newModul.ranges.push_back(pair<unsigned, unsigned>(modul->getInputs()[i], modul->getBitWidth()));
    }

    if (newModul.ranges.size() > 0 && newModul.ranges.back().first + newModul.ranges.back().second == modul->getStart()) {
        newModul.ranges.back().second += modul->getAdditionalVarCount();
    } else {
        newModul.ranges.push_back(pair<unsigned, unsigned>(modul->getStart(), modul->getAdditionalVarCount()));
    }

    for (vector<ModulEntry>::iterator it = module.begin(); it < module.end(); it++) {
        if ((*it).level > level) {
            module.insert(it, newModul);
            return;
        }
    }
    module.push_back(newModul);
}

bool ModulDB::isInSingleModul(vector<Lit>& clause, const char* fileprefix) {
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
        if (isInside) return true;
    }
    return false;
}
