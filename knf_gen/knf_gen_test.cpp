#include <cryptominisat4/cryptominisat.h>
#include <assert.h>
#include <stdio.h>

#include "minunit.h"

#include "module/bsig0_32.h"
#include "module/bsig1_32.h"
#include "module/ssig0_32.h"
#include "module/ssig1_32.h"
#include "module/maj_32.h"
#include "module/ch_32.h"
#include "module/adder_32.h"
#include "module/constadder_32.h"

MU_TEST_SUITE(test_suite) {
	//MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(Bsig0_32::test);
    MU_RUN_TEST(Bsig1_32::test);
    MU_RUN_TEST(Ssig0_32::test);
    MU_RUN_TEST(Ssig1_32::test);
    MU_RUN_TEST(Maj_32::test);
    MU_RUN_TEST(Ch_32::test);
	MU_RUN_TEST(Adder_32::test);
	MU_RUN_TEST(ConstAdder_32::test);
}

int main() {
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
    return 0;
}
