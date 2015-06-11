#ifndef __MAJ_32_H__
#define __MAJ_32_H__

#include "modul.h"
#include "../minunit.h"

class Maj_32 : public Modul {
    public:
        Maj_32();
        ~Maj_32();

        void create(Printer* printer);
        MU_TEST(test);
};

#endif //__MAJ_32_H__
