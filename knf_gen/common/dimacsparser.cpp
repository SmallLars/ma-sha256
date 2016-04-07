#include "dimacsparser.h"

#include <stdlib.h>

#include "clausetools.h"

using std::ifstream;
using std::string;
using std::vector;

using namespace CMSat;

DimacsParser::DimacsParser(const char* filename) {
    inputFile.open(filename);
}

DimacsParser::~DimacsParser() {
    inputFile.close();
}

bool DimacsParser::getNextClause(vector<Lit>& clause, bool xorAsOne) {
    clause.clear();
    if (popFromBuffer(clause)) return true;

    int c;
    while ((c = inputFile.peek()) != EOF) {
        if (c == 'c' || c == 'p') {
            string line;
            getline(inputFile, line);
            continue;
        }

        int value;
        bool isXor = false;
        if (c == 'x') {
            inputFile.get();
            isXor = true;
        }
        while (inputFile >> value && value != 0) {
            clause.push_back(Lit(abs(value) - 1, value < 0));
        }
        inputFile >> std::ws;

        if (isXor && !xorAsOne) {
            buffer = convertXOR(clause);
            popFromBuffer(clause);
        }
        return true;
    }
    return false;
}

void DimacsParser::reset() {
    inputFile.clear();
    inputFile.seekg(0);
}

bool DimacsParser::popFromBuffer(vector<Lit> &clause) {
    if (buffer.size() == 0) return false;

    clause = buffer.back();
    buffer.pop_back();
    return true;
}
