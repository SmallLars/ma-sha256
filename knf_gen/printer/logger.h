#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "fileprinter.h"

class Logger : public FilePrinter {
    public:
        Logger(const char* filename);
        ~Logger();

        void newModul(const char* name, Modul* modul);
};

#endif //__LOGGER_H__
