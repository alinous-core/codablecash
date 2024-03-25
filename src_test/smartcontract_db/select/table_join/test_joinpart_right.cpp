/*
 * test_joinpart_right.cpp
 *
 *  Created on: 2021/12/28
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base/StackRelease.h"

#include "smartcontract_db/toolkit/TestDbSchema01.h"
#include "engine/compiler/SmartContractParser.h"
#include "alinous_lang/AlinousLang.h"

#include "engine/sc_analyze/AnalyzeContext.h"

#include "scan_select/scan_planner/base/SelectScanPlanner.h"

#include "vm/VmSelectPlannerSetter.h"

#include "ext_binary/ExtDomArrayObject.h"
#include "ext_binary/ExtDomObject.h"

#include "base/UnicodeString.h"
using namespace codablecash;

TEST_GROUP(TestJoinPartRightGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestJoinPartRightGroup, case01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_join/resources/joinright/case01/select01.alns"))

	TestDbSchema01 schem(this->env);
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

		const ExtDomObject* domObj = dynamic_cast<const ExtDomObject*>(exobj);
		const UnicodeString* str = domObj->toString();

		UnicodeString ans0(L"{name: 'tanaka', id: 1, email: 'tanaka@tanaka.com', email_id: 11}");
		CHECK(str->equals(ans0));

		exobj = array->get(1);
		CHECK(exobj != nullptr);

		domObj = dynamic_cast<const ExtDomObject*>(exobj);
		str = domObj->toString();

		UnicodeString ans1(L"{name: 'tanaka', id: 1, email: 'yamada@yamada.com', email_id: 11}");
		CHECK(str->equals(ans1));
	}
}

TEST(TestJoinPartRightGroup, case02){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_join/resources/joinright/case02/select01.alns"))

	TestDbSchema01 schem(this->env);
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

		const ExtDomObject* domObj = dynamic_cast<const ExtDomObject*>(exobj);
		const UnicodeString* str = domObj->toString();

		UnicodeString ans0(L"{name: 'tanaka', id: 1, email: 'tanaka@tanaka.com', email_id: 11}");
		CHECK(str->equals(ans0));

		exobj = array->get(1);
		CHECK(exobj != nullptr);

		domObj = dynamic_cast<const ExtDomObject*>(exobj);
		str = domObj->toString();

		UnicodeString ans1(L"{name: 'tanaka', id: 1, email: 'yamada@yamada.com', email_id: 11}");
		CHECK(str->equals(ans1));
	}
}

TEST(TestJoinPartRightGroup, case03){// range(higher)
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_join/resources/joinright/case03/select01.alns"))

	TestDbSchema01 schem(this->env);
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

		const ExtDomObject* domObj = dynamic_cast<const ExtDomObject*>(exobj);
		const UnicodeString* str = domObj->toString();

		UnicodeString ans0(L"{name: 'tanaka', id: 1, email: 'tanaka@tanaka.com', email_id: 11}");
		CHECK(str->equals(ans0));

		exobj = array->get(1);
		CHECK(exobj != nullptr);

		domObj = dynamic_cast<const ExtDomObject*>(exobj);
		str = domObj->toString();

		UnicodeString ans1(L"{name: 'tanaka', id: 1, email: 'yamada@yamada.com', email_id: 11}");
		CHECK(str->equals(ans1));
	}
}

TEST(TestJoinPartRightGroup, case04){// range(lower)
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_join/resources/joinright/case04/select01.alns"))

	TestDbSchema01 schem(this->env);
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

		const ExtDomObject* domObj = dynamic_cast<const ExtDomObject*>(exobj);
		const UnicodeString* str = domObj->toString();

		UnicodeString ans0(L"{name: 'tanaka', id: 1, email: 'tanaka@tanaka.com', email_id: 11}");
		CHECK(str->equals(ans0));

		exobj = array->get(1);
		CHECK(exobj != nullptr);

		domObj = dynamic_cast<const ExtDomObject*>(exobj);
		str = domObj->toString();

		UnicodeString ans1(L"{name: 'tanaka', id: 1, email: 'yamada@yamada.com', email_id: 11}");
		CHECK(str->equals(ans1));
	}
}

TEST(TestJoinPartRightGroup, case05){// null join
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_join/resources/joinright/case05/select01.alns"))

	TestDbSchema01 schem(this->env);
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
		const UnicodeString* str = domObj->toString();

		UnicodeString ans0(L"{name: 'inoue', id: 7, email: null, email_id: null}");
		CHECK(str->equals(ans0));
	}
}

TEST(TestJoinPartRightGroup, case06){// null join
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_join/resources/joinright/case06/select01.alns"))

	TestDbSchema01 schem(this->env);
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
		CHECK(size == 0);
	}
}
