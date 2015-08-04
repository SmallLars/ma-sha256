#include "logger.h"

#include <iomanip>

#include "../module/modul.h"

using std::setw;
using namespace CMSat;

Logger::Logger(const char* filename) : FilePrinter(filename) {
    getStream() << "|        Name |              Inputs |     Start |    Output |\n";
    getStream() << "|-------------|---------------------|-----------|-----------|\n";
}

Logger::~Logger() {
}

void Logger::newModul(const char* name, Modul* modul) {
    getStream() << "| " << setw(11) << name << " | ";

    for (unsigned i = 0; i < 3; i++) {
        if (i < modul->getInputs().size()) {
            getStream() << setw(i == 0 ? 5 : 7) << modul->getInputs()[i] + 1;
        } else {
            if (i > 0) getStream() << "  ";
            getStream() << "     ";
        }
    }

    if (modul->getAdditionalVarCount() - modul->getOutputNum() == 0) {
        getStream() << " |          ";
    } else {
        getStream() << " | " << setw(5) << modul->getStart() + 1;
        getStream() <<          setw(4) << modul->getAdditionalVarCount() - modul->getOutputNum();
    }

    getStream() << " | " << setw(5) << modul->getOutput() + 1;
    getStream() <<          setw(4) << modul->getOutputNum();

    getStream() << " |\n" << std::flush;
}
