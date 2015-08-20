#ifndef __MODULDB_H__
#define __MODULDB_H__

#include "printer.h"

#include <utility>

struct ModulEntry {
    unsigned level;
    const char* name;
    std::vector< std::pair<unsigned, unsigned> > ranges;
};

class ModulDB : public Printer {
    public:
        ModulDB();
        ~ModulDB();

        void newModul(unsigned level, const char* name, Modul* modul);

        ModulEntry* isInSingleModul(std::vector<CMSat::Lit>& clause);
    private:
        std::vector<ModulEntry> module;
};

#endif //__MODULDB_H__
