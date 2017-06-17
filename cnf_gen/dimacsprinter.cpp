#include "module/sha256.h"

#ifdef XOR_SUPPORT
    #define FILENAME "1_sha256.dimacs"
#else
    #define FILENAME "0_sha256.dimacs"
#endif

int main() {
    Sha256 sha256;
    sha256.writeLog("sha256.txt");
    sha256.writeDimacs(FILENAME);
    std::cout << "Dimacs in Datei " << FILENAME << " geschrieben.\n";

    return 0;
}
