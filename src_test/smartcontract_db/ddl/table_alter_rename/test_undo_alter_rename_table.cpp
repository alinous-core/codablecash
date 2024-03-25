/*
 * test_undo_alter_rename_table.cpp
 *
 *  Created on: 2023/08/04
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "base/StackRelease.h"

#include "vm/VirtualMachine.h"

#include "engine/compiler/SmartContractParser.h"

#include "alinous_lang/AlinousLang.h"
#include "smartcontract_db/toolkit/TestDbSchema01.h"
#include "smartcontract_db/toolkit_alter//TestDbSchemaAlter01.h"
#include "smartcontract_db/toolkit_alter//TestDbSchemaAlter02.h"
#include "smartcontract_db/toolkit_alter//TestDbSchemaAlter03.h"

#include "engine/sc_analyze/AnalyzeContext.h"

#include "engine/CodableDatabase.h"

#include "schema_table/schema/SchemaManager.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/CdbTableColumn.h"
#include "schema_table/table/CdbTableIndex.h"

#include "snapshot/FileStoreSnapshot.h"

#include "trx/session/base/CdbDatabaseSessionId.h"

#include "snapshot/SnapShotSha256.h"
TEST_GROUP(TestUndoAlterRenameGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};

/**
 * ALTER TABLE test_table RENAME TO test_renamed;
 */
TEST(TestUndoAlterRenameGroup, renameTable01){
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
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_rename/resources/exec_rename/renameTable01.alns"))
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
 * ALTER TABLE test_table RENAME TO schema01.test_renamed;
 */
TEST(TestUndoAlterRenameGroup, renameTable02){
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
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_rename/resources/exec_rename/renameTable02.alns"))
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


