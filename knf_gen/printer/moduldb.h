#ifndef __MODULDB_H__
#define __MODULDB_H__

#include "printer.h"

#include <utility>

struct ModulEntry;

class ModulDB : public Printer {
    public:
        ModulDB();
        ~ModulDB();

        void newModul(unsigned level, const char* name, Modul* modul);

        bool isInSingleModul(std::vector<CMSat::Lit>& clause, const char* fileprefix);
    private:
        std::vector<ModulEntry> module;
};

#endif //__MODULDB_H__
