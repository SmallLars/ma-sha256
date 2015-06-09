#include <cryptominisat4/cryptominisat.h>
#include <assert.h>
#include <vector>
#include <stdio.h>

#include "module/const_32.h"
#include "module/bsig0_32.h"
#include "module/maj_32.h"
#include "module/adder_32.h"

using std::vector;
using namespace CMSat;

int main() {
    #include "module/bsig0_32_test.h"
    #include "module/maj_32_test.h"
    #include "module/adder_32_test.h"

    return 0;
}
