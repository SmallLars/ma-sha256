#ifndef __TTFILEPRINTER_H__
#define __TTFILEPRINTER_H__

#include "fileprinter.h"

class TTFilePrinter : public FilePrinter {
    public:
        TTFilePrinter(const char* filename, unsigned varCount);
        ~TTFilePrinter();

        void create(bool xOR, const std::vector<CMSat::Lit>& vars);
    protected:

    private:
        unsigned varCount;

        void printHeader();
};

#endif //__TTFILEPRINTER_H__
