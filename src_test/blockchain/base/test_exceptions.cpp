/*
 * test_exceptions.cpp
 *
 *  Created on: 2022/07/16
 *      Author: iizuka
 */

#include "bc/exceptions.h"
#include "test_utils/t_macros.h"

using codablecash::MulformattedTransactionBinaryException;

TEST_GROUP(TestExceptionsGroup) {
	TEST_SETUP(){
	}
	TEST_TEARDOWN(){
	}
};

TEST(TestExceptionsGroup, case01){
	testException<MulformattedTransactionBinaryException>();
}
