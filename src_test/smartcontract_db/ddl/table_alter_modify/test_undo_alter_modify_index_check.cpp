/*
 * test_undo_alter_modify_index_check.cpp
 *
 *  Created on: 2023/07/20
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "../../toolkit_alter/TestDbSchemaAlterModifyIndexCheck.h"
#include "engine/CodableDatabase.h"

#include "trx/session/base/CdbDatabaseSessionId.h"

#include "snapshot/FileStoreSnapshot.h"
#include "snapshot/SnapShotSha256.h"

#include "schema_table/table_store/IndexValidationException.h"
#include "schema_table/table_store/TableStore.h"

#include "schema_table/record/table_record_value/CdbOidValueList.h"

#include "engine/CdbOid.h"

using namespace codablecash;

TEST_GROUP(TestUndoAlterModifyIndexCheckGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};

TEST(TestUndoAlterModifyIndexCheckGroup, testex01){
	testException<IndexValidationException>();
}

TEST(TestUndoAlterModifyIndexCheckGroup, testex02){
	IndexValidationException* ex = nullptr;

	try{
		TableStore::__checkNotNull(nullptr);
	}catch(IndexValidationException* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestUndoAlterModifyIndexCheckGroup, testex03){
	IndexValidationException* ex = nullptr;

	try{
		CdbOidValueList list;
		list.addOid(new CdbOid(1L));
		list.addOid(new CdbOid(2L));

		TableStore::__checkIsUnique(&list);
	}catch(IndexValidationException* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

/**
 * OK case
 */
TEST(TestUndoAlterModifyIndexCheckGroup, case02){
	File testCaseFolder = this->env->testCaseDir();

	TestDbSchemaAlterModifyIndexCheck tester(env);
	tester.init(1024*10);
	tester.insert02();

	File* snapshotFile = testCaseFolder.get(L"snap.bin"); __STP(snapshotFile);
	File* snapshotFile2 = testCaseFolder.get(L"snap2.bin"); __STP(snapshotFile2);

	{
		/**
		 * ALTER TABLE test_table ADD UNIQUE INDEX test_index(email_id, comment);
		 */
		const File* projectFolder = this->env->getProjectRoot();
		_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_modify/resources/check_index/addIndex01.alns"))

		bool result = tester.execDDL(sourceFile);
		CHECK(result);

		const ExtExceptionObject* ex = tester.checkUncaughtException();
		CHECK(ex == nullptr);
	}

	CodableDatabase* db = tester.getDatabase();

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
		 * TYPE_STRING -> int
		 * ALTER TABLE test_table MODIFY comment int not null default 0;
		 */
		const File* projectFolder = this->env->getProjectRoot();
		_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_modify/resources/check_index/case01.alns"))

		bool result = tester.execDDL(sourceFile);
		CHECK(result);

		const ExtExceptionObject* ex = tester.checkUncaughtException();
		CHECK(ex == nullptr);
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

