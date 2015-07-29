#ifndef __FILEPRINTER_H__
#define __FILEPRINTER_H__

#include "printer.h"

#include <fstream>

class FilePrinter : public Printer {
    public:
        FilePrinter(const char* filename);
        ~FilePrinter();
    protected:
        std::ofstream& getStream();
    private:
        std::ofstream outputFile;
};

#endif //__FILEPRINTER_H__
