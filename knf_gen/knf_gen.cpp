#include <cryptominisat4/cryptominisat.h>
#include <assert.h>
#include <vector>
#include <stdio.h>

#include "module/const.h"
#include "module/bsig0.h"

using std::vector;
using namespace CMSat;

int main()
{
    SATSolver solver;
    solver.log_to_file("mylog.txt");
    solver.set_num_threads(4);

    solver.new_vars(64);

    uint32_t a = 0xabcdef98;

    Const ca(a);
    ca.append(&solver);

    Bsig0 bsig0;
    bsig0.append(&solver);

    lbool ret = solver.solve();
    assert(ret == l_True);

    for (unsigned i = 63; i >=32; i--) std::cout << (solver.get_model()[i] == l_True? 1 : 0); std::cout << std::endl;

    return 0;
}
