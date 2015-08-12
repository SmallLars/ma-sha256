#ifndef __MODULDB_H__
#define __MODULDB_H__

#include "printer.h"

#include <utility> 

class ModulDB : public Printer {
    public:
        ModulDB();
        ~ModulDB();

        void newModul(const char* name, Modul* modul);

        bool isInSingleModul(std::vector<CMSat::Lit>& clause);
    private:
        std::vector< std::vector< std::pair<unsigned, unsigned> > > module;
};

#endif //__MODULDB_H__
