#include "const_32.h"
#include "ssig1_32.h"

using namespace std;
using namespace CMSat;

class Ssig1_32_Test {
    public:
        Ssig1_32_Test() {}
        ~Ssig1_32_Test() {}

        bool test() {
            if (!ssig1(0xabcdef98)) return false;

            cout << "SSIG1 funktioniert." << endl;
            return true;
        }
    private:
        bool ssig1(unsigned a) {
            SATSolver solver;
            solver.log_to_file("test.log");
            solver.set_num_threads(4);

            uint32_t eingabe = a;
	        uint32_t ausgabe = (eingabe >> 17 | eingabe << (32-17)) ^ (eingabe >> 19 | eingabe << (32-19)) ^ (eingabe >> 10);
            uint32_t result = 0;

            Const_32 ca(eingabe);
            ca.append(&solver);

            Ssig1_32 ssig1;
            ssig1.append(&solver);

            lbool ret = solver.solve();
            assert(ret == l_True);

            for (unsigned i = 63; i >=32; i--) {
                result |= ((solver.get_model()[i] == l_True? 1 : 0) << (i - 32));
            }

            if (ausgabe != result) cout << "SSIG1 fail with (" << a << ")" << endl;
            return ausgabe == result;
        }
};
