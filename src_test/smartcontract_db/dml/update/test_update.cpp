/*
 * test_update.cpp
 *
 *  Created on: 2022/01/07
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "engine/compiler/SmartContractParser.h"
#include "alinous_lang/AlinousLang.h"

#include "lang_sql/sql_dml/UpdateStatement.h"

#include "../../toolkit/TestDbSchema01.h"
#include "ext_binary/ExtDomArrayObject.h"

#include "ext_binary/ExtDomObject.h"

#include "vm/VirtualMachine.h"

#include "engine/CodableDatabase.h"

#include "trx/transaction_log/UpdateLog.h"

#include "schema_table/record/table_record/CdbTableIdentifier.h"

#include "engine/CdbException.h"

#include "trx/transaction/CdbTransactionManager.h"
using namespace codablecash;

static const wchar_t* checkRecord(const ExtDomArrayObject* array, int pos){
	const AbstractExtObject* exobj = array->get(pos);

	const ExtDomObject* domObj = dynamic_cast<const ExtDomObject*>(exobj);
	const UnicodeString* str = domObj->toString();

	return str->towString();
}

TEST_GROUP(TestUpdateStatementGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestUpdateStatementGroup, case01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/dml/update/resources/update/case01/sql.alns"))
	_ST(File, selectFile, projectFolder->get(L"src_test/smartcontract_db/dml/update/resources/update/case01/select.alns"))

	TestDbSchema01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		UpdateStatement* stmt = lang->updateStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execUpdateStmt(stmt);
	}
	{
		/**
		 * SELECT * INTO res FROM test_table
			WHERE
				name = 'tanaka2';
		 */
		SmartContractParser parserSelect(selectFile);
		AlinousLang* lang = parserSelect.getDebugAlinousLang();

		SelectStatement* sstmt = lang->selectStatement(); __STP(sstmt);
		CHECK(!parserSelect.hasError())

		schem.execSelectStmt(sstmt);

		const AbstractExtObject* exobj = schem.getExtResultObj();
		const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

		int size = array->size();
		CHECK(size == 1);

		const wchar_t* ch = checkRecord(array, 0);
		UnicodeString ans(L"{name: 'tanaka2', id: 1, email_id: 11}");
		CHECK(ans.equals(ch));
	}
}

TEST(TestUpdateStatementGroup, case02){ // select updated
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/dml/update/resources/update/case02/sql.alns"))
	_ST(File, selectFile, projectFolder->get(L"src_test/smartcontract_db/dml/update/resources/update/case02/select.alns"))

	TestDbSchema01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();
	schem.begin();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		UpdateStatement* stmt = lang->updateStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execUpdateStmt(stmt);
	}

	{
		/**
		 * SELECT * INTO res FROM test_table
			WHERE
				name = 'tanaka2';
		 */
		SmartContractParser parserSelect(selectFile);
		AlinousLang* lang = parserSelect.getDebugAlinousLang();

		SelectStatement* sstmt = lang->selectStatement(); __STP(sstmt);
		CHECK(!parserSelect.hasError())

		schem.execSelectStmt(sstmt);

		const AbstractExtObject* exobj = schem.getExtResultObj();
		const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

		int size = array->size();
		CHECK(size == 1);

		const wchar_t* ch = checkRecord(array, 0);
		UnicodeString ans(L"{name: 'tanaka2', id: 1, email_id: 11}");
		CHECK(ans.equals(ch));
	}

	schem.commit();
}

TEST(TestUpdateStatementGroup, case03){ // select updated with index
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/dml/update/resources/update/case03/sql.alns"))
	_ST(File, selectFile, projectFolder->get(L"src_test/smartcontract_db/dml/update/resources/update/case03/select.alns"))

	TestDbSchema01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();
	schem.begin();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		UpdateStatement* stmt = lang->updateStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execUpdateStmt(stmt);
	}

	{
		/**
		 * SELECT * INTO res FROM test_table
			WHERE
				id = 1;
		 */
		SmartContractParser parserSelect(selectFile);
		AlinousLang* lang = parserSelect.getDebugAlinousLang();

		SelectStatement* sstmt = lang->selectStatement(); __STP(sstmt);
		CHECK(!parserSelect.hasError())

		schem.execSelectStmt(sstmt);

		const AbstractExtObject* exobj = schem.getExtResultObj();
		const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

		int size = array->size();
		CHECK(size == 1);

		const wchar_t* ch = checkRecord(array, 0);
		UnicodeString ans(L"{name: 'tanaka2', id: 1, email_id: 11}");
		CHECK(ans.equals(ch));
	}

	schem.commit();
}

TEST(TestUpdateStatementGroup, case04){ // select updated with index range
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/dml/update/resources/update/case04/sql.alns"))
	_ST(File, selectFile, projectFolder->get(L"src_test/smartcontract_db/dml/update/resources/update/case04/select.alns"))

	TestDbSchema01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();
	schem.begin();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		UpdateStatement* stmt = lang->updateStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execUpdateStmt(stmt);
	}

	{
		/**
		 * SELECT * INTO res FROM test_table
			WHERE
				id < 2;
		 */
		SmartContractParser parserSelect(selectFile);
		AlinousLang* lang = parserSelect.getDebugAlinousLang();

		SelectStatement* sstmt = lang->selectStatement(); __STP(sstmt);
		CHECK(!parserSelect.hasError())

		schem.execSelectStmt(sstmt);

		const AbstractExtObject* exobj = schem.getExtResultObj();
		const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

		int size = array->size();
		CHECK(size == 1);

		const wchar_t* ch = checkRecord(array, 0);
		UnicodeString ans(L"{name: 'tanaka2', id: 1, email_id: 11}");
		CHECK(ans.equals(ch));
	}

	schem.commit();
}

TEST(TestUpdateStatementGroup, updateInternal01){
	TestDbSchema01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();

	CodableDatabase* db = schem.getDatabase();
	CdbTransactionManager* mgr = db->getTransactionxManager();

	UpdateLog* cmd = new UpdateLog(1L); __STP(cmd);

	CdbTableIdentifier tid(L"public", L"aaaa");
	cmd->setTable(new CdbTableIdentifier(tid));

	CdbException* ex = nullptr;
	try{
		mgr->commitUpdate(cmd, nullptr, nullptr);
	}
	catch(CdbException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestUpdateStatementGroup, updateInternal02){
	TestDbSchema01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();

	CodableDatabase* db = schem.getDatabase();
	CdbTransactionManager* mgr = db->getTransactionxManager();

	UpdateLog* cmd = new UpdateLog(1L); __STP(cmd);

	CdbTableIdentifier tid(L"public222", L"aaaa");
	cmd->setTable(new CdbTableIdentifier(tid));

	CdbException* ex = nullptr;
	try{
		mgr->commitUpdate(cmd, nullptr, nullptr);
	}
	catch(CdbException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

