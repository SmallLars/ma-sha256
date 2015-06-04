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
        Modul(unsigned inputCount, unsigned inputBitWidth);
        ~Modul();

        unsigned getVarCount();
        unsigned getAdditionalVarCount();
        unsigned getClauseCount();

        void setInputs(const std::vector<unsigned>& inputs);
        void setStart(unsigned start);
        void setOutput(unsigned output);

        unsigned append(CMSat::SATSolver* solver);
        unsigned writeDimacs(const char* filename);
        unsigned writeTT(const char* filename);

        virtual void create(Printer* printer) = 0;
    protected:
        std::vector<unsigned> inputs;
        unsigned start;
        unsigned output;
    private:
        unsigned inputCount;
        unsigned inputBitWidth;
};

#endif //__MODUL_H__
