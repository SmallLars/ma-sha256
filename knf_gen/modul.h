#ifndef __MODUL_H__
#define __MODUL_H__

#include <vector>
#include <string>
#include <cryptominisat4/cryptominisat.h>

class Modul {
    public:
        Modul();
        ~Modul();

        void setStart(unsigned start);
        void setInputs(const std::vector<unsigned>& inputs);

        unsigned append(std::string filename);
        unsigned append(CMSat::SATSolver* solver);
    protected:
        virtual void create(void (Modul::*createX) (bool, const std::vector<CMSat::Lit>&)) = 0;
    private:
        unsigned start;
        std::vector<unsigned> inputs;
        CMSat::SATSolver* solver;
        void createF(bool xOR, const std::vector<CMSat::Lit>& vars);
        void createC(bool xOR, const std::vector<CMSat::Lit>& vars);
};

#endif //__MODUL_H__
