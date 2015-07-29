#ifndef __PRINTER_H__
#define __PRINTER_H__

#include <vector>

#include "cryptominisat4/cryptominisat.h"

class Modul;

class Printer {
    public:
        Printer();
        ~Printer();

        virtual void newModul(const char* name, Modul* modul);
        virtual void create(bool xOR, const std::vector<CMSat::Lit>& vars);
    protected:
        std::vector< std::vector<CMSat::Lit> > convertXOR(const std::vector<CMSat::Lit>& vars);
    private:

};

#endif //__PRINTER_H__
