#include "module/sha256.h"

#ifdef XOR_SUPPORT
    #ifdef XOR_OPTIMIZATION
        #define FILENAME "11_sha256.dimacs"
    #else
        #define FILENAME "10_sha256.dimacs"
    #endif
#else
    #ifdef XOR_OPTIMIZATION
        #define FILENAME "01_sha256.dimacs"
    #else
        #define FILENAME "00_sha256.dimacs"
    #endif
#endif

//#include "module/add_b1ch_32.h"
//#include "module/shacore_32.h"

int main() {
//    Add_B1Ch_32 add;
//    add.writeDimacs("add_b1ch_32.dimacs");

//    ShaCore_32 core;
//    core.writeDimacs("shacore_32.dimacs");

    Sha256 sha256;
    sha256.writeLog("sha256.txt");
    sha256.writeDimacs(FILENAME);
    std::cout << "Dimacs in Datei " << FILENAME << " geschrieben.\n";

    return 0;
}
