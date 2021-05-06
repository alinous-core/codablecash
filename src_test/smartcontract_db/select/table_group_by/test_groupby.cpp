/*
 * test_groupby.cpp
 *
 *  Created on: 2021/05/05
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base/StackRelease.h"

using namespace alinous;


TEST_GROUP(TestSelectGroupBy01Group) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};


TEST(TestSelectGroupBy01Group, case01){

}


