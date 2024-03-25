/*
 * test_undo_alter_primary.cpp
 *
 *  Created on: 2023/08/04
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "../../toolkit_alter/TestDbSchemaAlter01.h"
#include "engine/compiler/SmartContractParser.h"

#include "alinous_lang/AlinousLang.h"
#include "lang_sql/sql_ddl/AlterTableStatement.h"

#include "engine/sc_analyze/AnalyzeContext.h"

#include "snapshot/FileStoreSnapshot.h"
#include "snapshot/SnapShotSha256.h"

#include "trx/session/base/CdbDatabaseSessionId.h"

#include "engine/CodableDatabase.h"

#include "../../toolkit_alter/TestDbSchemaAlterTextUnique01.h"
using namespace codablecash;

TEST_GROUP(TestUndoAlterPrimaryGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};

TEST(TestUndoAlterPrimaryGroup, dropPrimaryKey01){
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
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_primary/resources/exec_primary/dropPrimaryKey01.alns"))
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
 * primary key is not unique
 * ALTER TABLE test_table DROP PRIMARY KEY;
 */
TEST(TestUndoAlterPrimaryGroup, dropPrimaryKey02){
	File testCaseFolder = this->env->testCaseDir();
	TestDbSchemaAlterTextUnique01 tester(this->env);
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
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_primary/resources/exec_primary/dropPrimaryKey01.alns"))
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
 *	add primary key(unique) after drop
 *	ALTER TABLE test_table ADD PRIMARY KEY(id);
 */
TEST(TestUndoAlterPrimaryGroup, addPrimaryKey01){
	File testCaseFolder = this->env->testCaseDir();
	TestDbSchemaAlter01 tester(this->env);
	tester.init(1024*10);
	tester.insert01();

	VirtualMachine* vm = tester.getVm();
	CodableDatabase* db = tester.getDatabase();

	File* snapshotFile = testCaseFolder.get(L"snap.bin"); __STP(snapshotFile);
	File* snapshotFile2 = testCaseFolder.get(L"snap2.bin"); __STP(snapshotFile2);

	// DROP FIRST
	{
		const File* projectFolder = this->env->getProjectRoot();
		_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_primary/resources/exec_primary/dropPrimaryKey01.alns"))
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
	}

	{
		FileStoreSnapshot handler;
		handler.openOutFile(snapshotFile);
		db->snapshot(&handler);

		handler.close();
	}

	CdbDatabaseSessionId* sessionId = CdbDatabaseSessionId::createRandomId(); __STP(sessionId);
	db->newSession(sessionId);

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_primary/resources/exec_primary/addPrimaryKey01.alns"))
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
 *	add primary key(not unique) after drop
 *	ALTER TABLE test_table ADD PRIMARY KEY(id);
 */
TEST(TestUndoAlterPrimaryGroup, addPrimaryKey02){
	File testCaseFolder = this->env->testCaseDir();
	TestDbSchemaAlterTextUnique01 tester(this->env);
	tester.init(1024*10);
	tester.insert01();

	VirtualMachine* vm = tester.getVm();
	CodableDatabase* db = tester.getDatabase();

	File* snapshotFile = testCaseFolder.get(L"snap.bin"); __STP(snapshotFile);
	File* snapshotFile2 = testCaseFolder.get(L"snap2.bin"); __STP(snapshotFile2);

	// DROP FIRST
	{
		const File* projectFolder = this->env->getProjectRoot();
		_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_primary/resources/exec_primary/dropPrimaryKey01.alns"))
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
	}

	{
		FileStoreSnapshot handler;
		handler.openOutFile(snapshotFile);
		db->snapshot(&handler);

		handler.close();
	}

	CdbDatabaseSessionId* sessionId = CdbDatabaseSessionId::createRandomId(); __STP(sessionId);
	db->newSession(sessionId);

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_primary/resources/exec_primary/addPrimaryKey01.alns"))
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

	CdbTableIndex* index = tester.getPrimaryKey(L"test_table");
	CHECK(index != nullptr);

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
