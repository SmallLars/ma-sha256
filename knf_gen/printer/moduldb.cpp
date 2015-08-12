#include "moduldb.h"

#include "../module/modul.h"

using std::vector;
using std::pair;
using namespace CMSat;

ModulDB::ModulDB() {
}

ModulDB::~ModulDB() {
/*
    for (unsigned m = 0; m < module.size(); m++) {
        for (unsigned range = 0; range < module[m].size(); range++) {
            std::cout << module[m][range].first << ":" << module[m][range].second << " ";
        }
        std::cout << "\n";
    }
*/
}

void ModulDB::newModul(const char* name, Modul* modul) {
    vector< pair<unsigned, unsigned> > newModul;

    for (unsigned i = 0; i < modul->getInputs().size(); i++) {
        newModul.push_back(pair<unsigned, unsigned>(modul->getInputs()[i], 32));
    }

    newModul.push_back(pair<unsigned, unsigned>(modul->getStart(), modul->getAdditionalVarCount()));
/*
    if (modul->getAdditionalVarCount() - modul->getOutputNum() != 0) {
        newModul.push_back(pair<unsigned, unsigned>(modul->getStart(), modul->getAdditionalVarCount() - modul->getOutputNum()));
    }
    newModul.push_back(pair<unsigned, unsigned>(modul->getOutput(), modul->getOutputNum()));
*/
    module.push_back(newModul);
}

bool ModulDB::isInSingleModul(vector<Lit>& clause) {
    for (unsigned m = 0; m < module.size(); m++) {
        bool isInside;
        for (unsigned lit = 0; lit < clause.size(); lit++) {
            isInside = false;
            for (unsigned range = 0; range < module[m].size(); range++) {
                if (clause[lit].var() >= module[m][range].first && clause[lit].var() < module[m][range].first + module[m][range].second) {
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
