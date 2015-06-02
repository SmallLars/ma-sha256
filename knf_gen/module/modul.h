#ifndef __MODUL_H__
#define __MODUL_H__

#include <vector>
#include <fstream>

#include "cryptominisat4/cryptominisat.h"

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
    protected:
        unsigned start;
        std::vector<unsigned> inputs;

        virtual void create(void (Modul::*createX) (bool, const std::vector<CMSat::Lit>&)) = 0;
    private:
        unsigned inputCount;
        std::ofstream outputFile;

        CMSat::SATSolver* solver;
        void createSolver(bool xOR, const std::vector<CMSat::Lit>& vars);
        void createDimacs(bool xOR, const std::vector<CMSat::Lit>& vars);
        void createTT(bool xOR, const std::vector<CMSat::Lit>& vars);
};

#endif //__MODUL_H__
