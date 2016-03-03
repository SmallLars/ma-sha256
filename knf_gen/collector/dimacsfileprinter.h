#ifndef __DIMACSFILEPRINTER_H__
#define __DIMACSFILEPRINTER_H__

#include "fileprinter.h"

class DimacsFilePrinter : public FilePrinter {
    public:
        DimacsFilePrinter(const char* filename, unsigned varCount, unsigned clauseCount);
        ~DimacsFilePrinter();

        void create(bool xOR, const std::vector<CMSat::Lit>& vars);
    protected:

    private:
        unsigned varCount;
        unsigned clauseCount;

        void printHeader();
};

#endif //__DIMACSFILEPRINTER_H__
