#include <cryptominisat4/cryptominisat.h>
#include <assert.h>
#include <stdio.h>

#include "module/bsig0_32_test.h"
#include "module/bsig1_32_test.h"
#include "module/ssig0_32_test.h"
#include "module/ssig1_32_test.h"
#include "module/maj_32_test.h"
#include "module/ch_32_test.h"
#include "module/adder_32_test.h"

int main() {
    Bsig0_32_Test bsig0Test;
    bsig0Test.test();

    Bsig1_32_Test bsig1Test;
    bsig1Test.test();

    Ssig0_32_Test ssig0Test;
    ssig0Test.test();

    Ssig1_32_Test ssig1Test;
    ssig1Test.test();

    Maj_32_Test majTest;
    majTest.test();

    Ch_32_Test chTest;
    chTest.test();

    Adder_32_Test addTest;
    addTest.test();

    return 0;
}
