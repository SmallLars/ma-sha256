#ifndef __DIMACSPARSER_H__
#define __DIMACSPARSER_H__

#include <fstream>
#include <vector>

#include "cryptominisat4/cryptominisat.h"

class DimacsParser {
    public:
        DimacsParser(const char* filename);
        ~DimacsParser();

        bool getNextClause(std::vector<CMSat::Lit> &clause, bool xorAsOne = false);
        void reset();
    protected:

    private:
        std::ifstream inputFile;
        std::vector< std::vector<CMSat::Lit> > buffer;

        bool popFromBuffer(std::vector<CMSat::Lit> &clause);
};

#endif //__DIMACSPARSER_H__
