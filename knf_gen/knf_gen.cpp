#include <assert.h>
#include <vector>
#include <stdio.h>

#include "cryptominisat4/cryptominisat.h"
#include "module/const_32.h"
#include "module/bsig0_32.h"
#include "module/maj_32.h"
#include "module/adder_32.h"
#include "module/constadder_32.h"
#include "module/ssig0_32.h"

using std::vector;
using namespace CMSat;

int main()
{
    SATSolver solver;
    solver.log_to_file("solver.log");
    solver.set_num_threads(4);

    Bsig0_32 bsig0;
    bsig0.writeDimacs("bsig0_32.dimacs");
    bsig0.writeTT("bsig0_32.tt");

    Maj_32 maj;
    maj.append(&solver);
    maj.writeDimacs("maj_32.dimacs");
    maj.writeTT("maj_32.tt");

    Adder_32 adder;
    adder.writeDimacs("adder_32.dimacs");
    adder.writeTT("adder_32.tt");

    ConstAdder_32 constadder(0xAAAAAAAA);
    constadder.writeDimacs("constadder_32.dimacs");
    constadder.writeTT("constadder_32.tt");

    Ssig0_32 ssig0;
    std::vector<unsigned> subinputs;
    subinputs.push_back(32);
    ssig0.setInputs(subinputs);
    ssig0.setStart(128);
    ssig0.writeDimacs("ssig0_32.dimacs");

    lbool ret = solver.solve();
    assert(ret == l_True);

    //for (unsigned i = 63; i >=32; i--) std::cout << (solver.get_model()[i] == l_True? 1 : 0); std::cout << std::endl;

    return 0;
}
