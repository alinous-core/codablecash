/*
 * test_scan_columns_arithmetic.cpp
 *
 *  Created on: 2021/12/09
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

#include "lang_sql/sql_dml_parts/SQLWhere.h"
using namespace codablecash;

TEST_GROUP(TestScanColumnsArithmeticGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestScanColumnsArithmeticGroup, caseplus01) {
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/arithmetic/caseplus01/select01.alns"))

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

		int32_t n = pr->getIntValue();
		CHECK(n == 5);
	}
}

TEST(TestScanColumnsArithmeticGroup, caseplus02) {
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/arithmetic/caseplus02/select01.alns"))

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

		int32_t n = pr->getIntValue();
		CHECK(n == 1);
	}
}

TEST(TestScanColumnsArithmeticGroup, casemul01) {
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/arithmetic/casemul01/select01.alns"))

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

		int32_t n = pr->getIntValue();
		CHECK(n == 6);
	}
}

TEST(TestScanColumnsArithmeticGroup, casemul02) {
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/arithmetic/casemul02/select01.alns"))

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

		int32_t n = pr->getIntValue();
		CHECK(n == 1);
	}
}

TEST(TestScanColumnsArithmeticGroup, casemul03) {
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/arithmetic/casemul03/select01.alns"))

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

		int32_t n = pr->getIntValue();
		CHECK(n == 2);
	}
}

TEST(TestScanColumnsArithmeticGroup, casemul04_err) {
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/arithmetic/casemul04_err/select01.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execSelectStmt(stmt);

		const ExtExceptionObject* ex = schem.getExeptionExObj();
		CHECK(ex != nullptr);

	}
}
