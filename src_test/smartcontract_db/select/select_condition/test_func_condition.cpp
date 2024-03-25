/*
 * test_func_condition.cpp
 *
 *  Created on: 2023/06/18
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "scan_select/scan_condition/func_string/SQLFuncUpper.h"

#include "scan_select/scan_condition/func_aggregate/CountSQLFunction.h"

#include "schema_table/record/table_record_value/CdbValueCastException.h"

#include "schema_table/record/table_record/CdbRecord.h"

using namespace codablecash;

TEST_GROUP(TestFuncConditionGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestFuncConditionGroup, case01){
	SQLFuncUpper upper;

	CHECK(upper.isAggregate() == false);
}

TEST(TestFuncConditionGroup, case02){
	CountSQLFunction func;

	CHECK(func.isAggregate() == true);

	CdbValueCastException* ex = nullptr;
	try{
		CdbRecord record;
		func.evaluate(nullptr, &record, nullptr, nullptr);
	}
	catch(CdbValueCastException* e){
		ex = e;
	}

	delete ex;
}
