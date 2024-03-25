/*
 * test_undo_delete.cpp
 *
 *  Created on: 2023/08/06
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
#include "engine/CdbException.h"

#include "trx/transaction_log/DeleteLog.h"

#include "schema_table/record/table_record/CdbTableIdentifier.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "snapshot/FileStoreSnapshot.h"
#include "snapshot/SnapShotSha256.h"

#include "trx/session/base/CdbDatabaseSessionId.h"

using namespace codablecash;

TEST_GROUP(TestUndoDeleteGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestUndoDeleteGroup, case01){
	File testCaseFolder = this->env->testCaseDir();
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/dml/delete/resources/delete/case01/sql.alns"))

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

	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		DeleteStatement* stmt = lang->deleteStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execDeleteStmt(stmt);
	}

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


