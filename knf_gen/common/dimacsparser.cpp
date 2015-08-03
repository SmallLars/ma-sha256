#include "dimacsparser.h"

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

bool DimacsParser::getNextClause(vector<Lit>& clause) {
    clause.clear();

    int c;
    while ((c = inputFile.peek()) != EOF) {
        if (c == 'c') {
            string line;
            getline(inputFile, line);
            continue;
        }

        int value;
        while (inputFile >> value && value != 0) {
            clause.push_back(Lit(abs(value) - 1, value < 0));
        }
        inputFile >> std::ws;
        return true;
    }
    return false;
}
