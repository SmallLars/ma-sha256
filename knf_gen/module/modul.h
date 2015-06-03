#ifndef __MODUL_H__
#define __MODUL_H__

#include <vector>

#include "../printer/printer.h"

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
        unsigned appendDimacs(const char* filename);
        unsigned appendTT(const char* filename);

        virtual void create(Printer* printer) = 0;
    protected:
        unsigned start;
        std::vector<unsigned> inputs;
    private:
        unsigned inputCount;
};

#endif //__MODUL_H__
