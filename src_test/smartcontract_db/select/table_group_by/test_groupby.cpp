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

#include "engine/compiler/SmartContractParser.h"

#include "lang_sql/sql_dml/SelectStatement.h"

#include "alinous_lang/AlinousLang.h"

#include "ext_binary/ExtDomArrayObject.h"
#include "ext_binary/ExtDomObject.h"
#include "ext_binary/ExtPrimitiveObject.h"

using namespace alinous;
using namespace codablecash;

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
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execSelectStmt(stmt);
	}
}

TEST(TestSelectGroupBy01Group, case02){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_group_by/resources/group01/case02/select01.alns"))

	TestDbSchemaGroupBy01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execSelectStmt(stmt);

		const AbstractExtObject* exobj = schem.getExtResultObj();
		const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

		int size = array->size();
		CHECK(size == 1);

		exobj = array->get(0);
		CHECK(exobj != nullptr);

		const ExtDomObject* domObj = dynamic_cast<const ExtDomObject*>(exobj);

		exobj = domObj->get(L"email_id");
		CHECK(exobj != nullptr);

		const ExtPrimitiveObject* primitive = dynamic_cast<const ExtPrimitiveObject*>(exobj);
		int64_t v = primitive->getLongValue();
		CHECK(v == 13)
	}
}

TEST(TestSelectGroupBy01Group, count01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_group_by/resources/group01/count01/select01.alns"))

	TestDbSchemaGroupBy01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execSelectStmt(stmt);

		const AbstractExtObject* exobj = schem.getExtResultObj();
		const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

		int size = array->size();
		CHECK(size == 3);

		exobj = array->get(0);
		CHECK(exobj != nullptr);
	}

}
