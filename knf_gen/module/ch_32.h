#ifndef __CH_32_H__
#define __CH_32_H__

#include "modul.h"
#include "../minunit.h"

class Ch_32 : public Modul {
    public:
        Ch_32();
        ~Ch_32();

        void create(Printer* printer);
        MU_TEST(test);
};

#endif //__CH_32_H__
