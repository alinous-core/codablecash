/*
 * test_insert.cpp
 *
 *  Created on: 2022/01/14
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "engine/compiler/SmartContractParser.h"
#include "alinous_lang/AlinousLang.h"

#include "lang_sql/sql_dml/UpdateStatement.h"
#include "lang_sql/sql_dml/InsertStatement.h"

#include "../../toolkit/TestDbSchema01.h"
#include "ext_binary/ExtDomArrayObject.h"

#include "ext_binary/ExtDomObject.h"

#include "vm/VirtualMachine.h"

#include "trx/transaction_log/InsertLog.h"

#include "engine/CodableDatabase.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "schema_table/record/table_record/CdbTableIdentifier.h"

#include "engine/CdbException.h"
using namespace codablecash;

static const wchar_t* checkRecord(const ExtDomArrayObject* array, int pos){
	const AbstractExtObject* exobj = array->get(pos);

	const ExtDomObject* domObj = dynamic_cast<const ExtDomObject*>(exobj);
	const UnicodeString* str = domObj->toString();

	return str->towString();
}


TEST_GROUP(TestInsertStatementGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestInsertStatementGroup, case01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/dml/insert/resources/insert/case01/sql.alns"))
	_ST(File, selectFile, projectFolder->get(L"src_test/smartcontract_db/dml/insert/resources/insert/case01/select.alns"))

	TestDbSchema01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		InsertStatement* stmt = lang->insertStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execInsertStmt(stmt);
	}
	{
		SmartContractParser parser(selectFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execSelectStmt(stmt);

		const AbstractExtObject* exobj = schem.getExtResultObj();
		const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

		int size = array->size();
		CHECK(size == 1);

		const wchar_t* ch = checkRecord(array, 0);
		UnicodeString ans(L"{name: 'tanaka2', id: 1000, email_id: 100}");
		CHECK(ans.equals(ch));
	}
}

TEST(TestInsertStatementGroup, case02){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/dml/insert/resources/insert/case02/sql.alns"))
	_ST(File, selectFile, projectFolder->get(L"src_test/smartcontract_db/dml/insert/resources/insert/case02/select.alns"))

	TestDbSchema01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();
	schem.begin();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		InsertStatement* stmt = lang->insertStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execInsertStmt(stmt);
	}
	{
		SmartContractParser parser(selectFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execSelectStmt(stmt);

		const AbstractExtObject* exobj = schem.getExtResultObj();
		const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

		int size = array->size();
		CHECK(size == 1);

		const wchar_t* ch = checkRecord(array, 0);
		UnicodeString ans(L"{name: 'tanaka2', id: 1000, email_id: 100}");
		CHECK(ans.equals(ch));
	}
	schem.commit();
}

TEST(TestInsertStatementGroup, case03){ // index scan
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/dml/insert/resources/insert/case03/sql.alns"))
	_ST(File, selectFile, projectFolder->get(L"src_test/smartcontract_db/dml/insert/resources/insert/case03/select.alns"))

	TestDbSchema01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();
	schem.begin();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		InsertStatement* stmt = lang->insertStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execInsertStmt(stmt);
	}
	{
		SmartContractParser parser(selectFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execSelectStmt(stmt);

		const AbstractExtObject* exobj = schem.getExtResultObj();
		const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

		int size = array->size();
		CHECK(size == 1);

		const wchar_t* ch = checkRecord(array, 0);
		UnicodeString ans(L"{name: 'tanaka2', id: 1000, email_id: 100}");
		CHECK(ans.equals(ch));
	}
	schem.commit();
}

TEST(TestInsertStatementGroup, case04){ // index range scan
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/dml/insert/resources/insert/case04/sql.alns"))
	_ST(File, selectFile, projectFolder->get(L"src_test/smartcontract_db/dml/insert/resources/insert/case04/select.alns"))

	TestDbSchema01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();
	schem.begin();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		InsertStatement* stmt = lang->insertStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execInsertStmt(stmt);
	}
	{
		SmartContractParser parser(selectFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execSelectStmt(stmt);

		const AbstractExtObject* exobj = schem.getExtResultObj();
		const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

		int size = array->size();
		CHECK(size == 1);

		const wchar_t* ch = checkRecord(array, 0);
		UnicodeString ans(L"{name: 'tanaka2', id: 1000, email_id: 100}");
		CHECK(ans.equals(ch));
	}
	schem.commit();
}

TEST(TestInsertStatementGroup, updateInternal01){
	TestDbSchema01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();

	CodableDatabase* db = schem.getDatabase();
	CdbTransactionManager* mgr = db->getTransactionxManager();

	InsertLog* cmd = new InsertLog(1L); __STP(cmd);

	CdbTableIdentifier tid(L"public", L"aaaa");
	cmd->setTable(new CdbTableIdentifier(tid));

	CdbException* ex = nullptr;
	try{
		mgr->commitInsert(cmd);
	}
	catch(CdbException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestInsertStatementGroup, updateInternal02){
	TestDbSchema01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();

	CodableDatabase* db = schem.getDatabase();
	CdbTransactionManager* mgr = db->getTransactionxManager();

	InsertLog* cmd = new InsertLog(1L); __STP(cmd);

	CdbTableIdentifier tid(L"public222", L"aaaa");
	cmd->setTable(new CdbTableIdentifier(tid));

	CdbException* ex = nullptr;
	try{
		mgr->commitInsert(cmd);
	}
	catch(CdbException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}


