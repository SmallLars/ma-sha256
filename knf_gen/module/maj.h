#ifndef __MAJ_H__
#define __MAJ_H__

#include "modul.h"

class Maj : public Modul {
    public:
        Maj();
        ~Maj();

        unsigned getAdditionalVarCount();
        unsigned getClauseCount();
    protected:
        void create(void (Modul::*createX) (bool, const std::vector<CMSat::Lit>&));
};

#endif //__MAJ_H__
