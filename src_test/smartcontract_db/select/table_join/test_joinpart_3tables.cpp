/*
 * test_joinpart_3tables.cpp
 *
 *  Created on: 2022/01/04
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

TEST_GROUP(TestJoinPart3TablesGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

static int countIdx(TestDbSchema3TablesJoin& schem) {
	IndexStore* istore = schem.getIndexStore(L"servers", L"server_id");

	CdbRecord* rec = new CdbRecord(); __STP(rec);
	rec->addValue(new CdbIntValue(20));
	CdbRecordKey* key = dynamic_cast<CdbRecordKey*>(rec->toKey()); __STP(key);

	IndexScanner* scanner = new IndexScanner(key, true, key, true, istore); __STP(scanner);
	scanner->start();

	int count = 0;
	while(scanner->hasNext()){
		const CdbOid* oid = scanner->next();

		int v = oid->getOidValue();
		count++;
	}

	return count;
}

TEST(TestJoinPart3TablesGroup, testTest01){
	TestDbSchema3TablesJoin schem(this->env);
	schem.init(1024*512);

/*
	schem.createTestTable1();
	schem.insertData1();

	schem.createTestTable2();
	schem.insertData2();

	schem.createTestTable3();
	schem.insertData3();
*/
	int count = countIdx(schem);
	CHECK(count == 1);

}

TEST(TestJoinPart3TablesGroup, leftJoin01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_join/resources/join3tables/leftJoin01/select01.alns"))

	TestDbSchema3TablesJoin schem(this->env);
	schem.init(1024*512);

	int count = countIdx(schem);
	CHECK(count == 1);

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

TEST(TestJoinPart3TablesGroup, innerJoin01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_join/resources/join3tables/innerJoin01/select01.alns"))

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

		domObj = dynamic_cast<const ExtDomObject*>(exobj);
		str = domObj->toString();

		UnicodeString ans1(L"{name: 'tanaka', id: 1, email: 'yamada@yamada.com', email_id: 11, server: 'yamada.com', server_id: 21}");
		CHECK(str->equals(ans1));
	}
}

TEST(TestJoinPart3TablesGroup, rightJoin01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_join/resources/join3tables/rightJoin01/select01.alns"))

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

TEST(TestJoinPart3TablesGroup, crossJoin01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_join/resources/join3tables/crossJoin01/select01.alns"))

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

