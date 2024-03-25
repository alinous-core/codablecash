/*
 * test_scan_columns_exp.cpp
 *
 *  Created on: 2021/12/11
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "base/StackRelease.h"

#include "../../toolkit/TestDbSchema02.h"
#include "engine/compiler/SmartContractParser.h"

#include "alinous_lang/AlinousLang.h"

#include "ext_binary/AbstractExtObject.h"
#include "ext_binary/ExtDomArrayObject.h"
#include "ext_binary/ExtDomObject.h"
#include "ext_binary/ExtPrimitiveObject.h"
#include "ext_binary/ExtStringClass.h"

#include "vm/VirtualMachine.h"

#include "ext_binary/ExtExceptionObject.h"

#include "lang/sc_statement/StatementBlock.h"

#include "engine/sc_analyze/AnalyzeContext.h"

using namespace codablecash;

TEST_GROUP(TestScanColumnsExpGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestScanColumnsExpGroup, caseEqual01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/exp/equal01/select01.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
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
		AbstractExtObject* col = domObj->get(L"col0");
		CHECK(col != nullptr);

		const ExtPrimitiveObject* pr = dynamic_cast<const ExtPrimitiveObject*>(col);
		CHECK(pr != nullptr);

		CHECK(pr->getBoolValue() == true)
	}
}

TEST(TestScanColumnsExpGroup, caseEqual02){ // parenthesis
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/exp/equal02/select01.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
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
		AbstractExtObject* col = domObj->get(L"col0");
		CHECK(col != nullptr);

		const ExtPrimitiveObject* pr = dynamic_cast<const ExtPrimitiveObject*>(col);
		CHECK(pr != nullptr);

		CHECK(pr->getBoolValue() == true)
	}
}

TEST(TestScanColumnsExpGroup, caseIsNull01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/exp/isnull01/select01.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
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
		AbstractExtObject* col = domObj->get(L"col0");
		CHECK(col != nullptr);

		const ExtPrimitiveObject* pr = dynamic_cast<const ExtPrimitiveObject*>(col);
		CHECK(pr != nullptr);

		CHECK(pr->getBoolValue() == true)
	}
}

TEST(TestScanColumnsExpGroup, caserel01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/exp/rel01/select01.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
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
		AbstractExtObject* col = domObj->get(L"col0");
		CHECK(col != nullptr);

		const ExtPrimitiveObject* pr = dynamic_cast<const ExtPrimitiveObject*>(col);
		CHECK(pr != nullptr);

		CHECK(pr->getBoolValue() == true)
	}
}

TEST(TestScanColumnsExpGroup, caserel02){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/exp/rel02/select01.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
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
		AbstractExtObject* col = domObj->get(L"col0");
		CHECK(col != nullptr);

		const ExtPrimitiveObject* pr = dynamic_cast<const ExtPrimitiveObject*>(col);
		CHECK(pr != nullptr);

		CHECK(pr->getBoolValue() == false)
	}
}

TEST(TestScanColumnsExpGroup, casebetween01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/exp/between01/select01.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
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
		AbstractExtObject* col = domObj->get(L"col0");
		CHECK(col != nullptr);

		const ExtPrimitiveObject* pr = dynamic_cast<const ExtPrimitiveObject*>(col);
		CHECK(pr != nullptr);

		CHECK(pr->getBoolValue() == true)
	}
}

TEST(TestScanColumnsExpGroup, casein01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/exp/in01/select01.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
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
		AbstractExtObject* col = domObj->get(L"col0");
		CHECK(col != nullptr);

		const ExtPrimitiveObject* pr = dynamic_cast<const ExtPrimitiveObject*>(col);
		CHECK(pr != nullptr);

		CHECK(pr->getBoolValue() == true)
	}
}

TEST(TestScanColumnsExpGroup, caselike01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/exp/like01/select01.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
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
		AbstractExtObject* col = domObj->get(L"col0");
		CHECK(col != nullptr);

		const ExtPrimitiveObject* pr = dynamic_cast<const ExtPrimitiveObject*>(col);
		CHECK(pr != nullptr);

		CHECK(pr->getBoolValue() == true)
	}
}

TEST(TestScanColumnsExpGroup, caseplaceholder01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/exp/placeholder01/select01.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();
		{
			VirtualMachine* vm = schem.getVm();
			AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
			actx->setVm(vm);

			CompilationUnit unit;

			StatementBlock* stmt = lang->statementBlock(); __STP(stmt);
			CHECK(!parser.hasError())

			stmt->setParent(&unit);

			stmt->preAnalyze(actx);
			stmt->analyzeTypeRef(actx);
			stmt->analyze(actx);

			stmt->init(vm);

			stmt->interpret(vm);
		}
	}
}
