#include "logger.h"

#include <iomanip>

#include "../module/modul.h"

using std::setw;
using namespace CMSat;

Logger::Logger(const char* filename) : FilePrinter(filename) {
    getStream() << "| Level |           Name |                     Inputs |     Start |    Output |\n";
    getStream() << "|-------|----------------|----------------------------|-----------|-----------|\n";
}

Logger::~Logger() {
}

void Logger::newModul(unsigned level, const char* name, Modul* modul) {
    if (level < 10) return;

    getStream() << "| " << setw(5) << level << " | ";
    getStream() << setw(14) << name << " | ";

    for (unsigned i = 0; i < 4; i++) {
        if (i == 3 && modul->getInputs().size() > 4) {
            getStream() << "  .....";
            break;
        }
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
