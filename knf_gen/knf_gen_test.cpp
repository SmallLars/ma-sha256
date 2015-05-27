#include <cryptominisat4/cryptominisat.h>
#include <assert.h>
#include <vector>
#include <stdio.h>

#include "module/const.h"
#include "module/bsig0.h"
#include "module/maj.h"

using std::vector;
using namespace CMSat;

int main() {
    #include "module/bsig0_test.h"
    #include "module/maj_test.h"

    return 0;
}
