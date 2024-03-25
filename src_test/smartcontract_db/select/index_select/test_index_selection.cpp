/*
 * test_index_selection.cpp
 *
 *  Created on: 2023/06/23
 *      Author: iizuka
 */
#include "../../toolkit_index/TestDbSchema2Index01.h"
#include "test_utils/t_macros.h"

#include "../../toolkit_index/TestDbSchema3Index01.h"
#include "engine/compiler/SmartContractParser.h"

#include "../../../../src_gen/alinous_lang/AlinousLang.h"
#include "lang_sql/sql_dml/SelectStatement.h"

#include "ext_binary/AbstractExtObject.h"

#include "ext_binary/ExtDomArrayObject.h"

#include "ext_binary/ExtDomObject.h"

#include "../../toolkit_index/TestDbSchema0Index01.h"
#include "scan_select/scan_planner/scanner/index_resolv/OrIndexAccessCollection.h"

#include "scan_select/scan_planner/scanner/index_resolv/MultipleColumnIndexAccess.h"

#include "engine/CodableDatabase.h"

#include "schema_table/schema/SchemaManager.h"

#include "schema_table/table/CdbTable.h"

#include "scan_select/scan_planner/scanner/index_resolv/SingleColumnIndexAccess.h"

#include "scan_select/scan_condition/params/NullScanParam.h"
using namespace codablecash;

TEST_GROUP(TestIndexSelectionGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestIndexSelectionGroup, case3cols01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/index_select/resources/select/case3cols01.alns"))

	TestDbSchema3Index01 schem(this->env);
	schem.init(1024*512);
	schem.insert01();
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

		UnicodeString ans0(L"{name: 'sato', id: 2, email_id: 12, comment: 'test2'}");
		CHECK(str->equals(ans0));
	}
}

TEST(TestIndexSelectionGroup, case2cols01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/index_select/resources/select/case2cols01.alns"))

	TestDbSchema2Index01 schem(this->env);
	schem.init(1024*512);
	schem.insert01();
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

		UnicodeString ans0(L"{name: 'sato', id: 2, email_id: 12, comment: 'test2'}");
		CHECK(str->equals(ans0));
	}
}

/**
 *  no index
 */
TEST(TestIndexSelectionGroup, case0cols01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/index_select/resources/select/case2cols01.alns"))

	TestDbSchema0Index01 schem(this->env);
	schem.init(1024*512);
	schem.insert01();
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

		UnicodeString ans0(L"{name: 'sato', id: 2, email_id: 12, comment: 'test2'}");
		CHECK(str->equals(ans0));
	}
}

TEST(TestIndexSelectionGroup, misc01){
	OrIndexAccessCollection col;

	CHECK(col.isBottomEq(nullptr) == false);
	CHECK(col.isTopEq(nullptr) == false);
	CHECK(col.getValue(nullptr) == nullptr);
	CHECK(col.getTopValue(nullptr) == nullptr);
}

TEST(TestIndexSelectionGroup, misc02){
	MultipleColumnIndexAccess access;

	TestDbSchema0Index01 schem(this->env);
	schem.init(1024*512);
	schem.insert01();

	CodableDatabase* db = schem.getDatabase();
	SchemaManager* scmagr = db->getSchemaManager();

	CdbTable* table = scmagr->getTable(L"public", L"test_table");
	CdbTableColumn* col0 = table->getColumn(L"name");
	CdbTableColumn* col1 = table->getColumn(L"email_id");

	NullScanParam nullParam;
	{
		SingleColumnIndexAccess* ac = new SingleColumnIndexAccess();
		ac->setColumn(col0);
		ac->setValue(&nullParam);
		access.add(ac);
	}

	{
		SingleColumnIndexAccess* ac = new SingleColumnIndexAccess();
		ac->setColumn(col1);
		ac->setValue(&nullParam);
		access.add(ac);
	}

	const UnicodeString* str = access.toCodeString(nullptr);
	UnicodeString ans0(L"name = null AND email_id = null");
	CHECK(str->equals(ans0));
}
