/*
 * test_scan_columns_logical.cpp
 *
 *  Created on: 2021/12/10
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
using namespace codablecash;

TEST_GROUP(TestScanColumnsLogicalGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestScanColumnsLogicalGroup, caseAnd01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/logical/caseand01/select01.alns"))

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

TEST(TestScanColumnsLogicalGroup, caseOr01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/logical/caseor01/select01.alns"))

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

TEST(TestScanColumnsLogicalGroup, caseNot01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/logical/casenot01/select01.alns"))

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

