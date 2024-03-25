/*
 * test_undo_create_table.cpp
 *
 *  Created on: 2023/07/08
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "engine/CodableDatabase.h"

#include "trx/transaction/CdbTransaction.h"

#include "trx/transaction_log/CreateTableLog.h"

#include "schema_table/table/CdbTable.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"

#include "trx/session/base/CdbDatabaseSessionId.h"

#include "snapshot/FileStoreSnapshot.h"

#include "snapshot/SnapShotSha256.h"
using codablecash::AbstractCdbValue;
using codablecash::CdbDatabaseSessionId;
using codablecash::CdbTable;
using codablecash::CdbTransaction;
using codablecash::CodableDatabase;
using codablecash::CreateTableLog;
using codablecash::FileStoreSnapshot;
using codablecash::SnapShotSha256;

TEST_GROUP(TestUnodoCreateTableGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestUnodoCreateTableGroup, case01){
	File testCaseFolder = this->env->testCaseDir();
	File* dbDir = testCaseFolder.get(L"db"); __STP(dbDir);
	File* undoDir = testCaseFolder.get(L"undo"); __STP(undoDir);

	File* snapshotFile = testCaseFolder.get(L"snap.bin"); __STP(snapshotFile);
	File* snapshotFile2 = testCaseFolder.get(L"snap2.bin"); __STP(snapshotFile2);

	CodableDatabase db;

	db.createDatabase(dbDir, undoDir);
	db.loadDatabase(dbDir, undoDir);

	{
		FileStoreSnapshot handler;
		handler.openOutFile(snapshotFile);
		db.snapshot(&handler);

		handler.close();
	}

	{
		CdbDatabaseSessionId* sessionId = CdbDatabaseSessionId::createRandomId(); __STP(sessionId);
		db.newSession(sessionId);

		CdbTransaction* trx = db.newTransaction(); __STP(trx);
		CHECK(trx != nullptr);

		CreateTableLog* cmd = new CreateTableLog(1L);
		CdbTable* table = new CdbTable(0);
		table->setName(new UnicodeString(L"test_table"));


		table->addColumn(10, L"id", AbstractCdbValue::TYPE_INT, 0, true, true, nullptr);
		table->addColumn(11, L"name", AbstractCdbValue::TYPE_INT, 0, true, true, L"");

		table->setPrimaryKey(L"id");

		cmd->setTable(table);

		trx->createTable(cmd);

		trx->commit();

		db.undoSession(sessionId);
	}

	{
		FileStoreSnapshot handler;
		handler.openOutFile(snapshotFile2);
		db.snapshot(&handler);

		handler.close();
	}

	SnapShotSha256 sha1(snapshotFile);
	SnapShotSha256 sha2(snapshotFile2);

	sha1.process();
	sha2.process();

	bool bl = sha1.equals(&sha2);
	CHECK(bl == true);
}
