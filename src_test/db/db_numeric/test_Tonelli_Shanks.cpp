/*
 * test_Tonelli_Shanks.cpp
 *
 *  Created on: 2023/01/30
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "numeric/BigInteger.h"

using namespace alinous;

TEST_GROUP(TestTonelliShanksGroup) {
	TEST_SETUP() {}
	TEST_TEARDOWN() {}
};

/*
print(square_root(67, 73))
# => "33" or "40"
print(square_root(4, 37))
# => "2" or "35"
print(square_root(6, 37))
# => "-1"
*/
TEST(TestTonelliShanksGroup, case01){
	BigInteger y(67);
	BigInteger p(73);

	BigInteger result((int64_t)0);
	bool bl = y.sqrtm(p, &result);
	CHECK(bl);

	int64_t ret = result.longValue();
	CHECK(ret == 33);

	BigInteger dbl = result.modPow(BigInteger::TWO, p);
	CHECK(dbl.compareTo(y) == 0);
}

TEST(TestTonelliShanksGroup, case02){
	BigInteger y(4);
	BigInteger p(37);

	BigInteger result((int64_t)0);
	bool bl = y.sqrtm(p, &result);
	CHECK(bl);

	int64_t ret = result.longValue();
	CHECK(ret == 2);

	BigInteger dbl = result.modPow(BigInteger::TWO, p);
	CHECK(dbl.compareTo(y) == 0);
}

TEST(TestTonelliShanksGroup, case03){
	BigInteger y(6);
	BigInteger p(37);

	BigInteger result((int64_t)0);
	bool bl = y.sqrtm(p, &result);
	CHECK(!bl);
}
/*
# (12603242261242278713399181313 - 1) = 2^66 * 3 * 17 * 83 * 40351
print(square_root(32189, 12603242261242278713399181313))
# => "4979547606364809208986559660" or "7623694654877469504412621653"
*/
/*
TEST(TestTonelliShanksGroup, case04){
	BigInteger y(32189);
	BigInteger p(L"12603242261242278713399181313", 10);

	BigInteger result((int64_t)0);
	bool bl = y.sqrtm(p, &result);
	CHECK(bl);

	int64_t ret = result.longValue();
	BigInteger ans(L"4979547606364809208986559660", 10);
	CHECK(ans.compareTo(result) == 0);
}
*/
