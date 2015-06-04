#ifndef __MAJ_32_H__
#define __MAJ_32_H__

#include "modul.h"

class Maj_32 : public Modul {
    public:
        Maj_32();
        ~Maj_32();

        unsigned getAdditionalVarCount();
        unsigned getClauseCount();

        void create(Printer* printer);
    protected:

};

#endif //__MAJ_32_H__
