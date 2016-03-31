#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include "cryptominisat4/cryptominisat.h"

#include "common/dimacsparser.h"

using std::cout;
using std::flush;
using std::vector;

using namespace CMSat;

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        cout << "Usage: clausecounter <FILE>" << "\n";
        return 0;
    }

    unsigned int clauses = 0;
    unsigned int counter = 0;

    DimacsParser parser(argv[1]);
    vector<Lit> clause;
    while (parser.getNextClause(clause, true)) {
        clauses++;
        counter += clause.size();
        cout << "\r" << clauses << "/" << counter << " : " << counter / (double) clauses;
    }

    cout << "\n";

    return 0;
}
