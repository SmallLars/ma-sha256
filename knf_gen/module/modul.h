#ifndef __MODUL_H__
#define __MODUL_H__

#include <vector>
#include <cmath>

#include "../printer/printer.h"

#ifdef XOR_SUPPORT
    #define XOR_CLAUSE_COUNT(n) 1
#else
    #define XOR_CLAUSE_COUNT(n) pow(2, n-1)
#endif

class Modul {
    public:
        Modul(unsigned inputCount);
        ~Modul();

        unsigned getVarCount();
        virtual unsigned getAdditionalVarCount() = 0;
        virtual unsigned getClauseCount() = 0;

        void setStart(unsigned start);
        void setInputs(const std::vector<unsigned>& inputs);

        unsigned append(CMSat::SATSolver* solver);
        unsigned writeDimacs(const char* filename);
        unsigned writeTT(const char* filename);

        virtual void create(Printer* printer) = 0;
    protected:
        unsigned start;
        std::vector<unsigned> inputs;
    private:
        unsigned inputCount;
};

#endif //__MODUL_H__
