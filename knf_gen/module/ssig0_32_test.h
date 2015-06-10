#include "const_32.h"
#include "ssig0_32.h"

using namespace std;
using namespace CMSat;

class Ssig0_32_Test {
    public:
        Ssig0_32_Test() {}
        ~Ssig0_32_Test() {}

        bool test() {
            if (!ssig0(0xabcdef98)) return false;

            cout << "SSIG0 funktioniert." << endl;
            return true;
        }
    private:
        bool ssig0(unsigned a) {
            SATSolver solver;
            solver.log_to_file("test.log");
            solver.set_num_threads(4);

            uint32_t eingabe = a;
	        uint32_t ausgabe = (eingabe >> 7 | eingabe << (32-7)) ^ (eingabe >> 18 | eingabe << (32-18)) ^ (eingabe >> 3);
            uint32_t result = 0;

            Const_32 ca(eingabe);
            ca.append(&solver);

            Ssig0_32 ssig0;
            ssig0.append(&solver);

            lbool ret = solver.solve();
            assert(ret == l_True);

            for (unsigned i = 63; i >=32; i--) {
                result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 32));
            }

            if (ausgabe != result) cout << "SSIG0 fail with (" << a << ")" << endl;
            return ausgabe == result;
        }
};
