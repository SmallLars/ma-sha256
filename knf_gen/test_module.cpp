#include <cryptominisat4/cryptominisat.h>
#include <assert.h>
#include <stdio.h>

#include "common/minunit.h"

#include "module/bsig0_32.h"
#include "module/bsig1_32.h"
#include "module/ssig0_32.h"
#include "module/ssig1_32.h"
#include "module/maj_32.h"
#include "module/ch_32.h"
#include "module/add_4.h"
#include "module/add_32.h"
#include "module/sub_32.h"
#include "module/constadd_32.h"
#include "module/add_ssig_32.h"
#include "module/add_prepare_32.h"
#include "module/add_b0maj_32.h"
#include "module/add_b1ch_32.h"
#include "module/prepare_32.h"
#include "module/shacore_32.h"
#include "module/shacore_ex1_32.h"
#include "module/shacore_ex2_32.h"

MU_TEST_SUITE(test_suite) {
	//MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(Bsig0_32::test);
    MU_RUN_TEST(Bsig1_32::test);
    MU_RUN_TEST(Ssig0_32::test);
    MU_RUN_TEST(Ssig1_32::test);
    MU_RUN_TEST(Maj_32::test);
    MU_RUN_TEST(Ch_32::test);
	MU_RUN_TEST(Add_4::test);
	MU_RUN_TEST(Add_32::test);
	MU_RUN_TEST(Sub_32::test);
	MU_RUN_TEST(ConstAdd_32::test);
	MU_RUN_TEST(Add_Ssig_32::test);
	MU_RUN_TEST(Add_Prepare_32::test);
	MU_RUN_TEST(Add_B0Maj_32::test);
	MU_RUN_TEST(Add_B1Ch_32::test);
	MU_RUN_TEST(Prepare_32::test);
	MU_RUN_TEST(ShaCore_32::test);
	MU_RUN_TEST(ShaCore_Ex1_32::test);
	MU_RUN_TEST(ShaCore_Ex2_32::test);
}

int main() {
//    Add_4 adder;
//    adder.writeTT("adder.tt");

	MU_RUN_SUITE(test_suite);
	MU_REPORT();
    return 0;
}
