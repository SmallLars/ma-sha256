#ifndef __SSIG1_32_H__
#define __SSIG1_32_H__

#include "modul.h"

class Ssig1_32 : public Modul {
    public:
        Ssig1_32();
        ~Ssig1_32();

        void create(Printer* printer);
};

#endif //__SSIG1_32_H__
