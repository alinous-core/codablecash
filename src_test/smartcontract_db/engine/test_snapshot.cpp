/*
 * test_snapshot.cpp
 *
 *  Created on: 2023/06/29
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "../toolkit_join/TestDbSchemaOrJoin01.h"

#include "snapshot/FileStoreSnapshot.h"
#include "snapshot/FileSnapshotRecoverer.h"
#include "snapshot/SnapShotSha256.h"
#include "snapshot/AbstractSnapshotCommand.h"

#include "engine/CodableDatabase.h"

#include "base_io/ByteBuffer.h"

using namespace codablecash;

TEST_GROUP(TestSnapshotGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};

TEST(TestSnapshotGroup, case01){
	File testCaseFolder = this->env->testCaseDir();
	File* snapshotFile = testCaseFolder.get(L"snap.bin"); __STP(snapshotFile);

	File* recoverdbDir = testCaseFolder.get(L"db2"); __STP(recoverdbDir);
	File* recoverdbUndoDir = testCaseFolder.get(L"db2undo"); __STP(recoverdbUndoDir);

	File* snapshotFile2 = testCaseFolder.get(L"snap2.bin"); __STP(snapshotFile2);


	TestDbSchemaOrJoin01 schem(this->env);
	schem.init(1024*512);

	CodableDatabase* db = schem.getDatabase();

	{
		FileStoreSnapshot handler;
		handler.openOutFile(snapshotFile);
		db->snapshot(&handler);

		handler.close();
	}

	{
		CodableDatabase db2;
		db2.createBlankDatabase(recoverdbDir, recoverdbUndoDir);
		db2.loadDatabase(recoverdbDir, recoverdbUndoDir);

		{
			FileSnapshotRecoverer recoverer;
			recoverer.open(snapshotFile);

			recoverer.recover(&db2);

			recoverer.close();
		}
		{
			FileStoreSnapshot handler;
			handler.openOutFile(snapshotFile2);
			db2.snapshot(&handler);

			handler.close();
		}
	}

	SnapShotSha256 sha1(snapshotFile);
	SnapShotSha256 sha2(snapshotFile2);

	sha1.process();
	sha2.process();

	bool bl = sha1.equals(&sha2);
	CHECK(bl == true);
}

TEST(TestSnapshotGroup, misk01){
	ByteBuffer* in = ByteBuffer::allocateWithEndian(10, true); __STP(in);
	in->put(100);
	in->position(0);

	AbstractSnapshotCommand* cmd = AbstractSnapshotCommand::createFromBinary(in);
	CHECK(cmd == 0);
}
