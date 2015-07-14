#include "add_4.h"

#include "clausecreator.h"

#include "const.h"

using namespace CMSat;

Add_4::Add_4() : Modul(4, 2, 1) {
    inputs.push_back(0);
    inputs.push_back(4);
    output = 8;
}

Add_4::~Add_4() {
}

void Add_4::create(Printer* printer) {
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
        solver.set_num_threads(4);

        uint32_t ausgabe = (a[t] + b[t]) & 0xF;
        uint32_t result = 0;

        Const con(4, a[t]);
        con.setOutput(0);
        con.append(&solver);

        con.setValue(b[t]);
        con.setOutput(4);
        con.append(&solver);

        Add_4 adder;
        adder.append(&solver);

        lbool ret = solver.solve();
        mu_assert(ret == l_True, "Adder UNSAT");

        for (unsigned i = 11; i >=8; i--) {
            result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 8));
        }

        mu_assert(ausgabe == result, "Adder failed");
    }
}
