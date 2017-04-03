#ifndef __FILEPRINTER_H__
#define __FILEPRINTER_H__

#include "collector.h"

#include <fstream>

class FilePrinter : public Collector {
    public:
        FilePrinter(const char* filename);
        ~FilePrinter();
    protected:
        std::ofstream& getStream();
    private:
        std::ofstream outputFile;
};

#endif //__FILEPRINTER_H__
