#include "logger.h"

#include <iomanip>

#include "../module/modul.h"

using std::setw;
using std::setfill;
using namespace CMSat;

Logger::Logger(const char* filename) : FilePrinter(filename) {
    getStream() << "| Level |           Name |                                 Inputs |     Start |   Output |\n";
    getStream() << "|-------|----------------|----------------------------------------|-----------|----------|\n";
}

Logger::~Logger() {
}

void Logger::newModul(unsigned level, const char* name, Modul* modul) {
    if (level < 10) return;

    getStream() << "| " << setfill(' ') << setw(5) << level << " | ";
    getStream() << setfill(' ') << setw(14) << name << " | ";

    for (unsigned i = 0; i < 4; i++) {
        if (i == 3 && modul->getInputs().size() > 4) {
            getStream() << "  ........";
            break;
        }
        if (i < modul->getInputs().size()) {
            getStream() << setfill(' ') << setw(i == 0 ? 5 : 7) << modul->getInputs()[i] + 1 << "-" << setfill('0') << setw(2) << modul->getInputWidth()[i];
        } else {
            if (i > 0) getStream() << "  ";
            getStream() << "        ";
        }
    }

    if (modul->getAdditionalVarCount() - modul->getOutputNum() == 0) {
        getStream() << " |          ";
    } else {
        getStream() << " | " << setfill(' ') << setw(5) << modul->getStart() + 1;
        getStream() << "-" << setfill('0') << setw(3) << modul->getAdditionalVarCount() - modul->getOutputNum();
    }

    getStream() << " | " << setfill(' ') << setw(5) << modul->getOutput() + 1;
    getStream() << "-" << setfill('0') << setw(2) << modul->getOutputNum();

    getStream() << " |\n" << std::flush;
}
