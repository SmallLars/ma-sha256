#include "const_32.h"
#include "maj_32.h"

using namespace std;
using namespace CMSat;

class Maj_32_Test {
    public:
        Maj_32_Test() {}
        ~Maj_32_Test() {}

        bool test() {
            if (!maj(0xabcdef98, 0x651d8fa1, 0x456af012)) return false;

            cout << "MAJ funktioniert." << endl;
            return true;
        }
    private:
        bool maj(unsigned a, unsigned b, unsigned c) {
            SATSolver solver;
            solver.log_to_file("test.log");
            solver.set_num_threads(4);

            uint32_t eingabe[3] = {a, b, c};
            uint32_t ausgabe = (eingabe[0] & eingabe[1]) ^ (eingabe[0] & eingabe[2]) ^ (eingabe[1] & eingabe[2]);
            uint32_t result = 0;

            Const_32 ca(eingabe[0]);
            ca.setOutput(0);
            ca.append(&solver);

            ca.setValue(eingabe[1]);
            ca.setOutput(32);
            ca.append(&solver);

            ca.setValue(eingabe[2]);
            ca.setOutput(64);
            ca.append(&solver);

            Maj_32 maj;
            maj.append(&solver);

            lbool ret = solver.solve();
            assert(ret == l_True);

            for (unsigned i = 127; i >=96; i--) {
                result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 96));
            }

            if (ausgabe != result) cout << "MAJ fail with (" << a << "," << b << "," << c << ")" << endl;
            return ausgabe == result;
        }
};
