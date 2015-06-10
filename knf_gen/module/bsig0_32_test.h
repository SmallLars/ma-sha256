#include "const_32.h"
#include "bsig0_32.h"

using namespace std;
using namespace CMSat;

class Bsig0_32_Test {
    public:
        Bsig0_32_Test() {}
        ~Bsig0_32_Test() {}

        bool test() {
            if (!bsig0(0xabcdef98)) return false;

            cout << "BSIG0 funktioniert." << endl;
            return true;
        }
    private:
        bool bsig0(unsigned a) {
            SATSolver solver;
            solver.log_to_file("test.log");
            solver.set_num_threads(4);

            uint32_t eingabe = a;
            uint32_t ausgabe = (eingabe >> 2 | eingabe << (32-2)) ^ (eingabe >> 13 | eingabe << (32-13)) ^ (eingabe >> 22 | eingabe << (32-22));
            uint32_t result = 0;

            Const_32 ca(eingabe);
            ca.append(&solver);

            Bsig0_32 bsig0;
            bsig0.append(&solver);

            lbool ret = solver.solve();
            assert(ret == l_True);

            for (unsigned i = 63; i >=32; i--) {
                result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 32));
            }

            if (ausgabe != result) cout << "BSIG0 fail with (" << a << ")" << endl;
            return ausgabe == result;
        }
};
