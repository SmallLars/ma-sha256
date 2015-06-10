#ifndef __CLAUSECREATOR_H__
#define __CLAUSECREATOR_H__

#include <vector>

#include "../printer/printer.h"

#define CC_DC 2

class ClauseCreator {
    public:
        ClauseCreator(Printer* printer);
        ~ClauseCreator();

        void setLiterals(unsigned n_args, ...);
        void printClause(unsigned n_args, ...);
    protected:

    private:
        std::vector<unsigned> literals;
        Printer* printer;
};

#endif //__CLAUSECREATOR_H__
