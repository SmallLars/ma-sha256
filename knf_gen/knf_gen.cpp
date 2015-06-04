#include <assert.h>
#include <vector>
#include <stdio.h>

#include "cryptominisat4/cryptominisat.h"
#include "module/const.h"
#include "module/bsig0.h"
#include "module/maj.h"

using std::vector;
using namespace CMSat;

int main()
{
    SATSolver solver;
    solver.log_to_file("solver.log");
    solver.set_num_threads(4);

    solver.new_vars(128);

    Bsig0 bsig0;
    bsig0.writeDimacs("bsig0.dimacs");
    bsig0.writeTT("bsig0.tt");

    Maj maj;
    maj.append(&solver);
    maj.writeDimacs("maj.dimacs");
    maj.writeTT("maj.tt");

    lbool ret = solver.solve();
    assert(ret == l_True);

    //for (unsigned i = 63; i >=32; i--) std::cout << (solver.get_model()[i] == l_True? 1 : 0); std::cout << std::endl;

    return 0;
}
