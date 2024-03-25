/*
 * test_joinpart_cross.cpp
 *
 *  Created on: 2022/01/02
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

TEST_GROUP(TestJoinPartCrossGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

static bool checkRecord(const ExtDomArrayObject* array, int pos, const wchar_t* recStr);

bool checkRecord(const ExtDomArrayObject* array, int pos, const wchar_t* recStr){
	const AbstractExtObject* exobj = array->get(pos);

	const ExtDomObject* domObj = dynamic_cast<const ExtDomObject*>(exobj);
	const UnicodeString* str = domObj->toString();

	UnicodeString ans0(recStr);
	return str->equals(ans0);
}

TEST(TestJoinPartCrossGroup, case01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_join/resources/joincross/case01/select01.alns"))

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
		CHECK(size == 14);

		bool bl = checkRecord(array, 0, L"{name: 'tanaka', id: 1, email: 'tanaka@tanaka.com', email_id: 11}"); CHECK(bl);
		bl = checkRecord(array, 1, L"{name: 'tanaka', id: 1, email: 'yamada@yamada.com', email_id: 11}"); CHECK(bl);
		bl = checkRecord(array, 2, L"{name: 'yamada', id: 2, email: 'tanaka@tanaka.com', email_id: 12}"); CHECK(bl);
		bl = checkRecord(array, 3, L"{name: 'yamada', id: 2, email: 'yamada@yamada.com', email_id: 12}"); CHECK(bl);
		bl = checkRecord(array, 4, L"{name: 'yamamoto', id: 3, email: 'tanaka@tanaka.com', email_id: 13}"); CHECK(bl);
		bl = checkRecord(array, 5, L"{name: 'yamamoto', id: 3, email: 'yamada@yamada.com', email_id: 13}"); CHECK(bl);
		bl = checkRecord(array, 6, L"{name: 'iizuka', id: 4, email: 'tanaka@tanaka.com', email_id: 14}"); CHECK(bl);
		bl = checkRecord(array, 7, L"{name: 'iizuka', id: 4, email: 'yamada@yamada.com', email_id: 14}"); CHECK(bl);
		bl = checkRecord(array, 8, L"{name: 'sato', id: 5, email: 'tanaka@tanaka.com', email_id: 15}"); CHECK(bl);
		bl = checkRecord(array, 9, L"{name: 'sato', id: 5, email: 'yamada@yamada.com', email_id: 15}"); CHECK(bl);
		bl = checkRecord(array, 10, L"{name: 'fujita', id: 6, email: 'tanaka@tanaka.com', email_id: 16}"); CHECK(bl);
		bl = checkRecord(array, 11, L"{name: 'fujita', id: 6, email: 'yamada@yamada.com', email_id: 16}"); CHECK(bl);
		bl = checkRecord(array, 12, L"{name: 'inoue', id: 7, email: 'tanaka@tanaka.com', email_id: 17}"); CHECK(bl);
		bl = checkRecord(array, 13, L"{name: 'inoue', id: 7, email: 'yamada@yamada.com', email_id: 17}"); CHECK(bl);
	}
}

TEST(TestJoinPartCrossGroup, case02){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_join/resources/joincross/case02/select01.alns"))

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
		CHECK(size == 14);

		bool bl = checkRecord(array, 0, L"{name: 'tanaka', id: 1, email: 'tanaka@tanaka.com', email_id: 11}"); CHECK(bl);
		bl = checkRecord(array, 1, L"{name: 'tanaka', id: 1, email: 'yamada@yamada.com', email_id: 11}"); CHECK(bl);
		bl = checkRecord(array, 2, L"{name: 'yamada', id: 2, email: 'tanaka@tanaka.com', email_id: 12}"); CHECK(bl);
		bl = checkRecord(array, 3, L"{name: 'yamada', id: 2, email: 'yamada@yamada.com', email_id: 12}"); CHECK(bl);
		bl = checkRecord(array, 4, L"{name: 'yamamoto', id: 3, email: 'tanaka@tanaka.com', email_id: 13}"); CHECK(bl);
		bl = checkRecord(array, 5, L"{name: 'yamamoto', id: 3, email: 'yamada@yamada.com', email_id: 13}"); CHECK(bl);
		bl = checkRecord(array, 6, L"{name: 'iizuka', id: 4, email: 'tanaka@tanaka.com', email_id: 14}"); CHECK(bl);
		bl = checkRecord(array, 7, L"{name: 'iizuka', id: 4, email: 'yamada@yamada.com', email_id: 14}"); CHECK(bl);
		bl = checkRecord(array, 8, L"{name: 'sato', id: 5, email: 'tanaka@tanaka.com', email_id: 15}"); CHECK(bl);
		bl = checkRecord(array, 9, L"{name: 'sato', id: 5, email: 'yamada@yamada.com', email_id: 15}"); CHECK(bl);
		bl = checkRecord(array, 10, L"{name: 'fujita', id: 6, email: 'tanaka@tanaka.com', email_id: 16}"); CHECK(bl);
		bl = checkRecord(array, 11, L"{name: 'fujita', id: 6, email: 'yamada@yamada.com', email_id: 16}"); CHECK(bl);
		bl = checkRecord(array, 12, L"{name: 'inoue', id: 7, email: 'tanaka@tanaka.com', email_id: 17}"); CHECK(bl);
		bl = checkRecord(array, 13, L"{name: 'inoue', id: 7, email: 'yamada@yamada.com', email_id: 17}"); CHECK(bl);
	}
}

TEST(TestJoinPartCrossGroup, case03){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_join/resources/joincross/case03/select01.alns"))

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

		exobj = array->get(1);
		CHECK(exobj != nullptr);

		domObj = dynamic_cast<const ExtDomObject*>(exobj);
		str = domObj->toString();

		UnicodeString ans1(L"{name: 'tanaka', id: 1, email: 'yamada@yamada.com', email_id: 11}");
		CHECK(str->equals(ans1));
	}
}

TEST(TestJoinPartCrossGroup, case04){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_join/resources/joincross/case04/select01.alns"))

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

		exobj = array->get(1);
		CHECK(exobj != nullptr);

		domObj = dynamic_cast<const ExtDomObject*>(exobj);
		str = domObj->toString();

		UnicodeString ans1(L"{name: 'tanaka', id: 1, email: 'yamada@yamada.com', email_id: 11}");
		CHECK(str->equals(ans1));
	}
}
