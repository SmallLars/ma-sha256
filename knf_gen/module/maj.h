#ifndef __MAJ_H__
#define __MAJ_H__

#include "modul.h"

class Maj : public Modul {
    public:
        Maj();
        ~Maj();

        unsigned getAdditionalVarCount();
        unsigned getClauseCount();

        void create(Printer* printer);
    protected:

};

#endif //__MAJ_H__
