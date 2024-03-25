/*
 * test_undo_insert.cpp
 *
 *  Created on: 2023/08/05
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "engine/compiler/SmartContractParser.h"
#include "alinous_lang/AlinousLang.h"

#include "lang_sql/sql_dml/UpdateStatement.h"
#include "lang_sql/sql_dml/InsertStatement.h"

#include "../../toolkit/TestDbSchema01.h"
#include "ext_binary/ExtDomArrayObject.h"
#include "ext_binary/ExtDomObject.h"

#include "vm/VirtualMachine.h"

#include "trx/transaction_log/InsertLog.h"

#include "engine/CodableDatabase.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "schema_table/record/table_record/CdbTableIdentifier.h"

#include "engine/CdbException.h"

#include "snapshot/FileStoreSnapshot.h"

#include "trx/session/base/CdbDatabaseSessionId.h"

#include "snapshot/SnapShotSha256.h"
using namespace codablecash;


TEST_GROUP(TestUndoInsertStatementGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestUndoInsertStatementGroup, case01){
	File testCaseFolder = this->env->testCaseDir();

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/dml/insert/resources/insert/case01/sql.alns"))
	_ST(File, selectFile, projectFolder->get(L"src_test/smartcontract_db/dml/insert/resources/insert/case01/select.alns"))

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

		InsertStatement* stmt = lang->insertStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execInsertStmt(stmt);
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
