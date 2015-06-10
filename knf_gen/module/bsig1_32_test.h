#include "const_32.h"
#include "bsig1_32.h"

using namespace std;
using namespace CMSat;

class Bsig1_32_Test {
    public:
        Bsig1_32_Test() {}
        ~Bsig1_32_Test() {}

        bool test() {
            if (!bsig1(0xabcdef98)) return false;

            cout << "BSIG1 funktioniert." << endl;
            return true;
        }
    private:
        bool bsig1(unsigned a) {
            SATSolver solver;
            solver.log_to_file("test.log");
            solver.set_num_threads(4);

            uint32_t eingabe = a;
            uint32_t ausgabe = (eingabe >> 6 | eingabe << (32-6)) ^ (eingabe >> 11 | eingabe << (32-11)) ^ (eingabe >> 25 | eingabe << (32-25));
            uint32_t result = 0;

            Const_32 ca(eingabe);
            ca.append(&solver);

            Bsig1_32 bsig1;
            bsig1.append(&solver);

            lbool ret = solver.solve();
            assert(ret == l_True);

            for (unsigned i = 63; i >=32; i--) {
                result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 32));
            }

            if (ausgabe != result) cout << "BSIG1 fail with (" << a << ")" << endl;
            return ausgabe == result;
        }
};
