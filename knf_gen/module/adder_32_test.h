#include "const_32.h"
#include "adder_32.h"

using namespace std;
using namespace CMSat;

class Adder_32_Test {
    public:
        Adder_32_Test() {}
        ~Adder_32_Test() {}

        bool test() {
            if (!add(1234, 1235)) return false;

            cout << "Adder funktioniert." << endl;
            return true;
        }
    private:
        bool add(unsigned a, unsigned b) {
            SATSolver solver;
            solver.log_to_file("test.log");
            solver.set_num_threads(4);

            uint32_t eingabe[2] = {a, b};
            uint32_t ausgabe = eingabe[0] + eingabe[1];
            uint32_t result = 0;

            Const_32 ca(eingabe[0]);
            ca.setOutput(0);
            ca.append(&solver);

            ca.setValue(eingabe[1]);
            ca.setOutput(32);
            ca.append(&solver);

            Adder_32 adder;
            adder.append(&solver);

            lbool ret = solver.solve();
            assert(ret == l_True);

            for (unsigned i = 126; i >=95; i--) {
                result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 95));
            }

            if (ausgabe != result) cout << "Adder fail with (" << a << "," << b << ")" << endl;
            return ausgabe == result;
        }
};
