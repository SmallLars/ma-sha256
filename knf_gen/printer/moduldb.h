#ifndef __MODULDB_H__
#define __MODULDB_H__

#include "printer.h"

#include <utility>
#include <iostream>

struct ModulEntry {
    unsigned level;
    const char* name;
    std::vector< std::pair<unsigned, unsigned> > ranges;

    void print(std::ostream& out);
};

class ModulDB : public Printer {
    public:
        ModulDB();
        ~ModulDB();

        void newModul(unsigned level, const char* name, Modul* modul);

        ModulEntry* isInSingleModul(std::vector<CMSat::Lit>& clause);
    private:
        std::vector<ModulEntry> module;
        unsigned lvl10_start;
};

#endif //__MODULDB_H__
