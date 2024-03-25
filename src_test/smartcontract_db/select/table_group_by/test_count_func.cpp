/*
 * test_count.cpp
 *
 *  Created on: 2021/11/02
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

#include "engine/sc_analyze/AnalyzeContext.h"
#include "engine/sc_analyze/ValidationError.h"

#include "engine/sc_analyze_stack/AnalyzeStackPopper.h"
#include "engine/sc_analyze_stack/AnalyzeStackManager.h"

#include "vm/VirtualMachine.h"

using namespace alinous;
using namespace codablecash;

TEST_GROUP(TestCountSQLFunc01Group) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestCountSQLFunc01Group, case01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_group_by/resources/count01/case01/select01.alns"))

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
		//printf("%d\n", size);
		CHECK(size == 2);

		exobj = array->get(0);
		CHECK(exobj != nullptr);
	}
}

TEST(TestCountSQLFunc01Group, case02){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_group_by/resources/count01/case02/select01.alns"))

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
		//printf("%d\n", size);
		CHECK(size == 1);

		exobj = array->get(0);
		CHECK(exobj != nullptr);
	}
}

TEST(TestCountSQLFunc01Group, case03){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_group_by/resources/count01/case03/select01.alns"))

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
		//printf("%d\n", size);
		CHECK(size == 2);

		exobj = array->get(0);
		CHECK(exobj != nullptr);
	}
}

TEST(TestCountSQLFunc01Group, case04){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_group_by/resources/count01/case04/select01.alns"))

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
		CHECK(size == 2);

		exobj = array->get(0);
		CHECK(exobj != nullptr);
	}
}

TEST(TestCountSQLFunc01Group, case05_err){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_group_by/resources/count01/case05_err/select01.alns"))

	TestDbSchemaGroupBy01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		{
			AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
			actx->setVm(vm);

			AnalyzeStackManager* stackMgr = actx->getAnalyzeStackManager();
			AnalyzeStackPopper popper(stackMgr, true);
			stackMgr->addFunctionStack();

			stmt->preAnalyze(actx);
			stmt->analyzeTypeRef(actx);
			stmt->analyze(actx);

			bool bl = vm->hasAnalyzeError(ValidationError::SQL_CONDITION_WRONG_DISTINCT_USE, actx);
			CHECK(bl)
		}
	}
}

TEST(TestCountSQLFunc01Group, case06_err){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_group_by/resources/count01/case06_err/select01.alns"))

	TestDbSchemaGroupBy01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		{
			AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
			actx->setVm(vm);

			AnalyzeStackManager* stackMgr = actx->getAnalyzeStackManager();
			AnalyzeStackPopper popper(stackMgr, true);
			stackMgr->addFunctionStack();

			stmt->preAnalyze(actx);
			stmt->analyzeTypeRef(actx);
			stmt->analyze(actx);

			bool bl = vm->hasAnalyzeError(ValidationError::SQL_CONDITION_WRONG_FUNC_NAME, actx);
			CHECK(bl)
		}
	}
}

TEST(TestCountSQLFunc01Group, case07_err){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_group_by/resources/count01/case07_err/select01.alns"))

	TestDbSchemaGroupBy01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		{
			AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
			actx->setVm(vm);

			AnalyzeStackManager* stackMgr = actx->getAnalyzeStackManager();
			AnalyzeStackPopper popper(stackMgr, true);
			stackMgr->addFunctionStack();

			stmt->preAnalyze(actx);
			stmt->analyzeTypeRef(actx);
			stmt->analyze(actx);

			bool bl = vm->hasAnalyzeError(ValidationError::SQL_CONDITION_WRONG_FUNC_ARG_NUM, actx);
			CHECK(bl)
		}
	}
}
