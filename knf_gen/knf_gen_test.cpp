#include <cryptominisat4/cryptominisat.h>
#include <assert.h>
#include <vector>
#include <stdio.h>

#include "const.h"
#include "bsig0.h"

using std::vector;
using namespace CMSat;

int main() {
    #include "bsig0_test.cpp"

    return 0;
}
