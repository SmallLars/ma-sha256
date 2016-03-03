#ifndef __COLLECTOR_H__
#define __COLLECTOR_H__

#include <vector>

#include "cryptominisat4/cryptominisat.h"

class Modul;

class Collector {
    public:
        Collector();
        ~Collector();

        virtual void newModul(unsigned level, const char* name, Modul* modul);
        virtual void create(bool xOR, const std::vector<CMSat::Lit>& vars);
    protected:
        std::vector< std::vector<CMSat::Lit> > convertXOR(const std::vector<CMSat::Lit>& vars);
    private:

};

#endif //__COLLECTOR_H__
