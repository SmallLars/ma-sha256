#include "fileprinter.h"

#include <iostream>

FilePrinter::FilePrinter(const char* filename) {
    outputFile.open(filename, std::ios::out | std::ios::trunc);
}

FilePrinter::~FilePrinter() {
    outputFile.close();
}

std::ofstream& FilePrinter::getStream() {
    return outputFile;
}
