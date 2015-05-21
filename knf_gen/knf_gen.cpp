#include <cryptominisat4/cryptominisat.h>
#include <assert.h>
#include <vector>
using std::vector;
using namespace CMSat;

int main()
{
    SATSolver solver;
    vector<Lit> clause;
    vector<unsigned> x_clause;

    solver.log_to_file("mylog.txt");

    //Let's use 4 threads
    solver.set_num_threads(4);



    //  BSIG0(x) = ROTR^2(x) XOR ROTR^13(x) XOR ROTR^22(x)

    // 32 Eingangsvariablen (0 bis 31) + 32 Ausgangsvariablen (32 bis 63)
    solver.new_vars(64);

    for (unsigned i = 0; i < 32; i++) {
        x_clause.clear();
        x_clause.push_back(32 + i);
        x_clause.push_back((i +  2) % 32);
        x_clause.push_back((i + 13) % 32);
        x_clause.push_back((i + 22) % 32);
        solver.add_xor_clause(x_clause, false);
    }

/*
    //adds "1 0"
    clause.push_back(Lit(0, false));
    solver.add_clause(clause);

    //adds "-2 0"
    clause.clear();
    clause.push_back(Lit(1, true));
    solver.add_clause(clause);

    //adds "-1 2 3 0"
    clause.clear();
    clause.push_back(Lit(0, true));
    clause.push_back(Lit(1, false));
    clause.push_back(Lit(2, false));
    solver.add_clause(clause);

    //xortest
    x_clause.clear();
    x_clause.push_back(3);
    x_clause.push_back(4);
    x_clause.push_back(5);
    solver.add_xor_clause(x_clause, true);
*/

    lbool ret = solver.solve();
/*
    assert(ret == l_True);
    assert(solver.get_model()[0] == l_True);
    assert(solver.get_model()[1] == l_False);
    assert(solver.get_model()[2] == l_True);
*/
    std::cout
    << "Solution is: "
    << solver.get_model()[0]
    << ", " << solver.get_model()[1]
    << ", " << solver.get_model()[2]
    << std::endl;

    return 0;
}
