#ifndef __DIMACSPARSER_H__
#define __DIMACSPARSER_H__

#include <fstream>
#include <vector>

#include "cryptominisat4/cryptominisat.h"

class DimacsParser {
    public:
        DimacsParser(const char* filename);
        ~DimacsParser();

        bool getNextClause(std::vector<CMSat::Lit> &clause, bool doXor = false);
        void reset();
    protected:

    private:
        std::ifstream inputFile;
};

#endif //__DIMACSPARSER_H__
