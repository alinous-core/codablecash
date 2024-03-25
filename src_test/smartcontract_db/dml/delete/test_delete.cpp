/*
 * test_delete.cpp
 *
 *  Created on: 2022/01/07
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "engine/compiler/SmartContractParser.h"
#include "alinous_lang/AlinousLang.h"

#include "lang_sql/sql_dml/DeleteStatement.h"

#include "../../toolkit/TestDbSchema01.h"
#include "ext_binary/ExtDomArrayObject.h"

#include "engine/CodableDatabase.h"

#include "trx/transaction_log/DeleteLog.h"

#include "schema_table/record/table_record/CdbTableIdentifier.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "engine/CdbException.h"
using namespace codablecash;

TEST_GROUP(TestDeleteStatementGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestDeleteStatementGroup, case01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/dml/delete/resources/delete/case01/sql.alns"))
	_ST(File, selectFile, projectFolder->get(L"src_test/smartcontract_db/dml/delete/resources/delete/case01/select.alns"))

	TestDbSchema01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		DeleteStatement* stmt = lang->deleteStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execDeleteStmt(stmt);
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
		CHECK(size == 0);
	}
}

TEST(TestDeleteStatementGroup, deleteInserted01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, insertFile, projectFolder->get(L"src_test/smartcontract_db/dml/delete/resources/delete/deleteInserted01/insert.alns"))
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/dml/delete/resources/delete/deleteInserted01/sql.alns"))
	_ST(File, selectFile, projectFolder->get(L"src_test/smartcontract_db/dml/delete/resources/delete/deleteInserted01/select.alns"))

	TestDbSchema01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();

	schem.begin();
	{
		SmartContractParser parser(insertFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		InsertStatement* stmt = lang->insertStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execInsertStmt(stmt);
	}
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		DeleteStatement* stmt = lang->deleteStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execDeleteStmt(stmt);
	}
	{
		/**
		 * SELECT * INTO res FROM test_table
			WHERE
				id = 1000;
		 */
		SmartContractParser parserSelect(selectFile);
		AlinousLang* lang = parserSelect.getDebugAlinousLang();

		SelectStatement* sstmt = lang->selectStatement(); __STP(sstmt);
		CHECK(!parserSelect.hasError())

		schem.execSelectStmt(sstmt);

		const AbstractExtObject* exobj = schem.getExtResultObj();
		const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

		int size = array->size();
		CHECK(size == 0);
	}

	schem.commit();
}

TEST(TestDeleteStatementGroup, deleteInserted02){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, insertFile, projectFolder->get(L"src_test/smartcontract_db/dml/delete/resources/delete/deleteInserted01/insert.alns"))
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/dml/delete/resources/delete/deleteInserted01/sql.alns"))
	_ST(File, selectFile, projectFolder->get(L"src_test/smartcontract_db/dml/delete/resources/delete/deleteInserted01/select.alns"))

	TestDbSchema01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();

	schem.begin();
	{
		SmartContractParser parser(insertFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		InsertStatement* stmt = lang->insertStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execInsertStmt(stmt);
	}
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		DeleteStatement* stmt = lang->deleteStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execDeleteStmt(stmt);
	}
	schem.commit();

	{
		/**
		 * SELECT * INTO res FROM test_table
			WHERE
				id = 1000;
		 */
		SmartContractParser parserSelect(selectFile);
		AlinousLang* lang = parserSelect.getDebugAlinousLang();

		SelectStatement* sstmt = lang->selectStatement(); __STP(sstmt);
		CHECK(!parserSelect.hasError())

		schem.execSelectStmt(sstmt);

		const AbstractExtObject* exobj = schem.getExtResultObj();
		const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

		int size = array->size();
		CHECK(size == 0);
	}
}

TEST(TestDeleteStatementGroup, deleteInternal01){
	TestDbSchema01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();

	CodableDatabase* db = schem.getDatabase();
	CdbTransactionManager* mgr = db->getTransactionxManager();

	DeleteLog* cmd = new DeleteLog(1L); __STP(cmd);

	CdbTableIdentifier tid(L"public", L"aaaa");
	cmd->setTable(new CdbTableIdentifier(tid));

	CdbException* ex = nullptr;
	try{
		mgr->commitDelete(cmd, nullptr, nullptr);
	}
	catch(CdbException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestDeleteStatementGroup, deleteInternal02){
	TestDbSchema01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();

	CodableDatabase* db = schem.getDatabase();
	CdbTransactionManager* mgr = db->getTransactionxManager();

	DeleteLog* cmd = new DeleteLog(1L); __STP(cmd);

	CdbTableIdentifier tid(L"public222", L"aaaa");
	cmd->setTable(new CdbTableIdentifier(tid));

	CdbException* ex = nullptr;
	try{
		mgr->commitDelete(cmd, nullptr, nullptr);
	}
	catch(CdbException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}
