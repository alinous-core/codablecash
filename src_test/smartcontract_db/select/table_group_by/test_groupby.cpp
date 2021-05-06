/*
 * test_groupby.cpp
 *
 *  Created on: 2021/05/05
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base/StackRelease.h"

#include "smartcontract_vm/VmTestUtils.h"

#include "../../toolkit_groupby/TestDbSchemaGroupBy01.h"

using codablecash::TestDbSchemaGroupBy01;

using namespace alinous;
//using namespace codablecash;

TEST_GROUP(TestSelectGroupBy01Group) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};


TEST(TestSelectGroupBy01Group, case01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_group_by/resources/group01/case01/select01.alns"))

	TestDbSchemaGroupBy01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();
	{

	}
}


