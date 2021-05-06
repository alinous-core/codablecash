/*
 * TestDbSchemaGroupBy01.cpp
 *
 *  Created on: 2021/05/06
 *      Author: iizuka
 */

#include "TestDbSchemaGroupBy01.h"

namespace codablecash {

TestDbSchemaGroupBy01::TestDbSchemaGroupBy01(TestEnv* env) : TestDbSchemaBase(env) {

}

TestDbSchemaGroupBy01::~TestDbSchemaGroupBy01() {

}

void TestDbSchemaGroupBy01::init() {
	TestDbSchemaBase::init();
}

void TestDbSchemaGroupBy01::init(uint64_t memCapacity) {
	TestDbSchemaBase::init(memCapacity);

	//createTable();
	//insertData();
}

} /* namespace codablecash */
