#include "add_4.h"

#include "clausecreator.h"

#include "../common/solvertools.h"

using namespace CMSat;

unsigned Add_4::stats[STATS_LENGTH];

Add_4::Add_4() : Modul(4, 2, 1) {
    inputs.push_back(0);
    inputs.push_back(4);
    output = 8;
}

Add_4::~Add_4() {
}

unsigned* Add_4::getStats() {
    return stats;
}

void Add_4::create(Printer* printer) {
    printer->newModul(2, "Add_4", this);

    ClauseCreator cc(printer);

    cc.setLiterals(12, inputs[0] + 0, inputs[0] + 1, inputs[0] + 2, inputs[0] + 3,
                       inputs[1] + 0, inputs[1] + 1, inputs[1] + 2, inputs[1] + 3,
                       output + 0, output + 1, output + 2, output + 3);

    for (unsigned i = 0; i < 4096; i++) {
        if (((((i >> 0) & 0xF) + ((i >> 4) & 0xF)) & 0xF) != ((i >> 8) & 0xF)) {
            cc.printClause(12, !(i & 0x001), !(i & 0x002), !(i & 0x004), !(i & 0x008),
                               !(i & 0x010), !(i & 0x020), !(i & 0x040), !(i & 0x080),
                               !(i & 0x100), !(i & 0x200), !(i & 0x400), !(i & 0x800));
        }
    }
}

MU_TEST_C(Add_4::test) {
    unsigned a[] = {0, 1, 2, 3, 4,  5,  6, 7,  8};
    unsigned b[] = {5, 1, 3, 4, 2, 12, 10, 9, 12};

    for (unsigned t = 0; t < 10; t++) {
        SATSolver solver;
        solver.log_to_file("test.log");

        uint32_t ausgabe = (a[t] + b[t]) & 0xF;

        solver_writeInt(solver, 0, 4, a[t]);
        solver_writeInt(solver, 4, 4, b[t]);

        Add_4 adder;
        adder.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "Adder UNSAT");
        mu_assert(ausgabe == solver_readInt(solver, 8, 4), "Adder failed");
    }
}
