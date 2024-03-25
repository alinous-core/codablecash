/*
 * test_undo_alter_column.cpp
 *
 *  Created on: 2023/07/31
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "lang_sql/sql_ddl/AlterTableStatement.h"

#include "engine/compiler/SmartContractParser.h"

#include "engine/sc_analyze/AnalyzeContext.h"

#include "../../../../src_gen/alinous_lang/AlinousLang.h"
#include "../../toolkit_alter/TestDbSchemaAlter01.h"

#include "trx/session/base/CdbDatabaseSessionId.h"

#include "engine/CodableDatabase.h"

#include "snapshot/SnapShotSha256.h"
#include "snapshot/FileStoreSnapshot.h"

#include "schema_table/table_store/TableStore.h"

#include "schema_table/table/CdbTable.h"

#include "schema_table/table/CdbTableIndex.h"

#include "schema_table/schema/SchemaManager.h"

#include "schema_table/table_store/CdbStorageManager.h"
using namespace codablecash;

TEST_GROUP(TestUndoAlterColumnGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};

/**
 * add column
 * ALTER TABLE test_table ADD COLUMN email_id2 int not null unique default '0';
 */
TEST(TestUndoAlterColumnGroup, addColumn01){
	File testCaseFolder = this->env->testCaseDir();
	TestDbSchemaAlter01 tester(this->env);
	tester.init(1024*10);
	tester.insert01();

	VirtualMachine* vm = tester.getVm();
	CodableDatabase* db = tester.getDatabase();

	File* snapshotFile = testCaseFolder.get(L"snap.bin"); __STP(snapshotFile);
	File* snapshotFile2 = testCaseFolder.get(L"snap2.bin"); __STP(snapshotFile2);

	{
		FileStoreSnapshot handler;
		handler.openOutFile(snapshotFile);
		db->snapshot(&handler);

		handler.close();
	}

	CdbDatabaseSessionId* sessionId = CdbDatabaseSessionId::createRandomId(); __STP(sessionId);
	db->newSession(sessionId);

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_column/resources/exec_column/addColumn01.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		AlterTableStatement* stmt = lang->alterTableStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		stmt->preAnalyze(actx);
		stmt->analyzeTypeRef(actx);
		stmt->analyze(actx);

		stmt->interpret(vm);
	}

	db->__testCheckIndexes();

	db->undoSession(sessionId);

	db->__testCheckIndexes();

	{
		FileStoreSnapshot handler;
		handler.openOutFile(snapshotFile2);
		db->snapshot(&handler);

		handler.close();
	}

	SnapShotSha256 sha1(snapshotFile);
	SnapShotSha256 sha2(snapshotFile2);

	sha1.process();
	sha2.process();

	bool bl = sha1.equals(&sha2);
	CHECK(bl == true);
}

/**
 * drop column
 * ALTER TABLE test_table DROP email_id;
 */
TEST(TestUndoAlterColumnGroup, dropColumn01){
	File testCaseFolder = this->env->testCaseDir();
	TestDbSchemaAlter01 tester(this->env);
	tester.init(1024*10);
	tester.insert01();

	VirtualMachine* vm = tester.getVm();
	CodableDatabase* db = tester.getDatabase();

	File* snapshotFile = testCaseFolder.get(L"snap.bin"); __STP(snapshotFile);
	File* snapshotFile2 = testCaseFolder.get(L"snap2.bin"); __STP(snapshotFile2);

	{
		FileStoreSnapshot handler;
		handler.openOutFile(snapshotFile);
		db->snapshot(&handler);

		handler.close();
	}

	CdbDatabaseSessionId* sessionId = CdbDatabaseSessionId::createRandomId(); __STP(sessionId);
	db->newSession(sessionId);

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_column/resources/exec_column/dropColumn01.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		AlterTableStatement* stmt = lang->alterTableStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		stmt->preAnalyze(actx);
		stmt->analyzeTypeRef(actx);
		stmt->analyze(actx);

		stmt->interpret(vm);
	}

	db->__testCheckIndexes();

	db->undoSession(sessionId);

	db->__testCheckIndexes();

	{
		FileStoreSnapshot handler;
		handler.openOutFile(snapshotFile2);
		db->snapshot(&handler);

		handler.close();
	}

	SnapShotSha256 sha1(snapshotFile);
	SnapShotSha256 sha2(snapshotFile2);

	sha1.process();
	sha2.process();

	bool bl = sha1.equals(&sha2);
	CHECK(bl == true);
}

TEST(TestUndoAlterColumnGroup, case01){
	File testCaseFolder = this->env->testCaseDir();
	TestDbSchemaAlter01 tester(this->env);
	tester.init(1024*10);
	tester.insert03();

	SchemaManager* schemaManager = tester.getSchemaManager();
	CdbStorageManager* storageManager = tester.getStorageManager();

	CodableDatabase* db = tester.getDatabase();

	File* snapshotFile = testCaseFolder.get(L"snap.bin"); __STP(snapshotFile);
	File* snapshotFile2 = testCaseFolder.get(L"snap2.bin"); __STP(snapshotFile2);

	{
		/**
		 * ALTER TABLE test_table ADD INDEX test_index(id, email_id);
		 */
		const File* projectFolder = this->env->getProjectRoot();
		_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_column/resources/drop/case01_1.alns"))

		bool result = tester.execDDL(sourceFile);
		CHECK(result);
	}

	{
		FileStoreSnapshot handler;
		handler.openOutFile(snapshotFile);
		db->snapshot(&handler);

		handler.close();
	}

	CdbDatabaseSessionId* sessionId = CdbDatabaseSessionId::createRandomId(); __STP(sessionId);
	db->newSession(sessionId);

	{
		/**
		 * ALTER TABLE test_table DROP email_id;
		 */
		const File* projectFolder = this->env->getProjectRoot();
		_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_column/resources/drop/case01.alns"))

		bool result = tester.execDDL(sourceFile);
		CHECK(result);

		CdbTable* table = schemaManager->getTable(L"public", L"test_table");
		TableStore* store = storageManager->getTableStore(table->getOid());

		UnicodeString indexname(L"test_index");
		CdbTableIndex* idx = table->getIndexByName(&indexname);

		CHECK(idx == nullptr);
	}

	db->__testCheckIndexes();

	db->undoSession(sessionId);

	db->__testCheckIndexes();

	{
		FileStoreSnapshot handler;
		handler.openOutFile(snapshotFile2);
		db->snapshot(&handler);

		handler.close();
	}

	SnapShotSha256 sha1(snapshotFile);
	SnapShotSha256 sha2(snapshotFile2);

	sha1.process();
	sha2.process();

	bool bl = sha1.equals(&sha2);
	CHECK(bl == true);
}
