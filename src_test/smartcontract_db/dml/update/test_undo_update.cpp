/*
 * test_undo_update.cpp
 *
 *  Created on: 2023/08/06
 *      Author: iizuka
 */

#include "../../toolkit/TestDbSchema01.h"
#include "test_utils/t_macros.h"
#include "vm/VirtualMachine.h"

#include "engine/compiler/SmartContractParser.h"

#include "../../../../src_gen/alinous_lang/AlinousLang.h"
#include "snapshot/FileStoreSnapshot.h"

#include "engine/CodableDatabase.h"

#include "trx/session/base/CdbDatabaseSessionId.h"

#include "snapshot/SnapShotSha256.h"
using codablecash::TestDbSchema01;

TEST_GROUP(TestUndoUpdateGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestUndoUpdateGroup, case01){
	File testCaseFolder = this->env->testCaseDir();

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, insertFile, projectFolder->get(L"src_test/smartcontract_db/dml/update/resources/insert/case01/insert.alns"))
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/dml/update/resources/insert/case01/sql.alns"))
	_ST(File, selectFile, projectFolder->get(L"src_test/smartcontract_db/dml/update/resources/insert/case01/select.alns"))

	TestDbSchema01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();
	CodableDatabase* db = schem.getDatabase();

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

	schem.begin();
	{
		SmartContractParser parser(insertFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		InsertStatement* stmt = lang->insertStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execInsertStmt(stmt);
	}
	{
		/**
		UPDATE test_table
		SET
			name = 'tanaka3'
		WHERE
			id = 1000;
		 */
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		UpdateStatement* stmt = lang->updateStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execUpdateStmt(stmt);
	}
	schem.commit();

	db->undoSession(sessionId);

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
