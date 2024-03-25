/*
 * test_or_join_3tables.cpp
 *
 *  Created on: 2022/01/29
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "engine/compiler/SmartContractParser.h"
#include "alinous_lang/AlinousLang.h"

#include "engine/sc_analyze/AnalyzeContext.h"

#include "scan_select/scan_planner/base/SelectScanPlanner.h"

#include "scan_select/scan/IndexScanner.h"

#include "vm/VmSelectPlannerSetter.h"

#include "ext_binary/ExtDomArrayObject.h"
#include "ext_binary/ExtDomObject.h"

#include "../../toolkit_join/TestDbSchema3TablesJoin.h"
#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record_value/CdbIntValue.h"

#include "schema_table/record/table_record_key/CdbRecordKey.h"

#include "engine/CdbOid.h"
using namespace codablecash;

TEST_GROUP(TestJoinOr3TablesGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestJoinOr3TablesGroup, case01){ // LEFT JOIN
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_join/resources/orjoin3tables/case01/select01.alns"))

	TestDbSchema3TablesJoin schem(this->env);
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

		UnicodeString ans0(L"{name: 'tanaka', id: 1, email: 'tanaka@tanaka.com', email_id: 11, server: 'tanaka.com', server_id: 20}");
		CHECK(str->equals(ans0));

		exobj = array->get(1);
		CHECK(exobj != nullptr);

		exobj = array->get(1);
		CHECK(exobj != nullptr);

		domObj = dynamic_cast<const ExtDomObject*>(exobj);
		str = domObj->toString();

		UnicodeString ans1(L"{name: 'tanaka', id: 1, email: 'yamada@yamada.com', email_id: 11, server: 'yamada.com', server_id: 21}");
		CHECK(str->equals(ans1));
	}
}

TEST(TestJoinOr3TablesGroup, case02){ // INNER JOIN
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_join/resources/orjoin3tables/case02/select01.alns"))

	TestDbSchema3TablesJoin schem(this->env);
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

		UnicodeString ans0(L"{name: 'tanaka', id: 1, email: 'tanaka@tanaka.com', email_id: 11, server: 'tanaka.com', server_id: 20}");
		CHECK(str->equals(ans0));

		exobj = array->get(1);
		CHECK(exobj != nullptr);

		exobj = array->get(1);
		CHECK(exobj != nullptr);

		domObj = dynamic_cast<const ExtDomObject*>(exobj);
		str = domObj->toString();

		UnicodeString ans1(L"{name: 'tanaka', id: 1, email: 'yamada@yamada.com', email_id: 11, server: 'yamada.com', server_id: 21}");
		CHECK(str->equals(ans1));
	}
}

TEST(TestJoinOr3TablesGroup, case03){ // INNER JOIN 2
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_join/resources/orjoin3tables/case03/select01.alns"))

	TestDbSchema3TablesJoin schem(this->env);
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

		UnicodeString ans0(L"{name: 'tanaka', id: 1, email: 'tanaka@tanaka.com', email_id: 11, server: 'tanaka.com', server_id: 20}");
		CHECK(str->equals(ans0));

		exobj = array->get(1);
		CHECK(exobj != nullptr);

		exobj = array->get(1);
		CHECK(exobj != nullptr);

		domObj = dynamic_cast<const ExtDomObject*>(exobj);
		str = domObj->toString();

		UnicodeString ans1(L"{name: 'tanaka', id: 1, email: 'yamada@yamada.com', email_id: 11, server: 'yamada.com', server_id: 21}");
		CHECK(str->equals(ans1));
	}
}

TEST(TestJoinOr3TablesGroup, case04){ // RIGHT JOIN
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_join/resources/orjoin3tables/case04/select01.alns"))

	TestDbSchema3TablesJoin schem(this->env);
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

		UnicodeString ans0(L"{name: 'tanaka', id: 1, email: 'tanaka@tanaka.com', email_id: 11, server: 'tanaka.com', server_id: 20}");
		CHECK(str->equals(ans0));

		exobj = array->get(1);
		CHECK(exobj != nullptr);

		exobj = array->get(1);
		CHECK(exobj != nullptr);

		domObj = dynamic_cast<const ExtDomObject*>(exobj);
		str = domObj->toString();

		UnicodeString ans1(L"{name: 'tanaka', id: 1, email: 'yamada@yamada.com', email_id: 11, server: 'yamada.com', server_id: 21}");
		CHECK(str->equals(ans1));
	}
}

TEST(TestJoinOr3TablesGroup, case05){ // CROSS JOIN
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_join/resources/orjoin3tables/case05/select01.alns"))

	TestDbSchema3TablesJoin schem(this->env);
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
		CHECK(size == 4);
	}
}
