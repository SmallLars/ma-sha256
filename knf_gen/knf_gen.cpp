#include <assert.h>
#include <vector>
#include <stdio.h>

#include "cryptominisat4/cryptominisat.h"
#include "module/const_32.h"
#include "module/bsig0_32.h"
#include "module/maj_32.h"

using std::vector;
using namespace CMSat;

int main()
{
    SATSolver solver;
    solver.log_to_file("solver.log");
    solver.set_num_threads(4);

    solver.new_vars(128);

    Bsig0_32 bsig0;
    bsig0.writeDimacs("bsig0_32.dimacs");
    bsig0.writeTT("bsig0_32.tt");

    Maj_32 maj;
    maj.append(&solver);
    maj.writeDimacs("maj_32.dimacs");
    maj.writeTT("maj_32.tt");

    lbool ret = solver.solve();
    assert(ret == l_True);

    //for (unsigned i = 63; i >=32; i--) std::cout << (solver.get_model()[i] == l_True? 1 : 0); std::cout << std::endl;

    return 0;
}
