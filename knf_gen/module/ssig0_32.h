#ifndef __SSIG0_32_H__
#define __SSIG0_32_H__

#include "modul.h"

class Ssig0_32 : public Modul {
    public:
        Ssig0_32();
        ~Ssig0_32();

        void create(Printer* printer);
};

#endif //__SSIG0_32_H__
