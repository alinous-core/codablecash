/*
 * test_like_processor.cpp
 *
 *  Created on: 2022/01/30
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base/StackRelease.h"

#include "scan_select/scan_condition/exp_like/LikeExpressionProcessor.h"

using namespace codablecash;

TEST_GROUP(TestLikeProcessorGroup) {
	TEST_SETUP(){
	}
	TEST_TEARDOWN(){
	}
};

TEST(TestLikeProcessorGroup, case01){
	UnicodeString str(L"test%aaa");
	LikeExpressionProcessor processor(&str);

	processor.compile();

	bool bl = processor.match(L"test2aaa");
	CHECK(bl);
	bl = processor.match(L"test2aaaa");
	CHECK(bl);
	bl = processor.match(L"test2bbb");
	CHECK(!bl);
	bl = processor.match(L"tet2bbb");
	CHECK(!bl);
}

TEST(TestLikeProcessorGroup, case02){
	UnicodeString str(L"%a%");
	LikeExpressionProcessor processor(&str);

	processor.compile();

	bool bl = processor.match(L"aaaa");
	CHECK(bl);
}

TEST(TestLikeProcessorGroup, case03){
	UnicodeString str(L"abc");
	LikeExpressionProcessor processor(&str);

	processor.compile();

	bool bl = processor.match(L"abc");
	CHECK(bl);
	bl = processor.match(L"abcd");
	CHECK(!bl);
}

TEST(TestLikeProcessorGroup, case04){
	UnicodeString str(L"*a*");
	LikeExpressionProcessor processor(&str);

	UnicodeString esc(L"*");
	processor.setEscape(&esc);

	processor.compile();

	bool bl = processor.match(L"aaaa");
	CHECK(bl);
}

