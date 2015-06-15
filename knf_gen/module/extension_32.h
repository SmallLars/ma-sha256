#ifndef __EXTENSION_32_H__
#define __EXTENSION_32_H__

#include "modul.h"
#include "../minunit.h"

class Extension_32 : public Modul {
    public:
        Extension_32();
        ~Extension_32();

        void create(Printer* printer);
        MU_TEST_H(test);
};

#endif //__EXTENSION_32_H__
