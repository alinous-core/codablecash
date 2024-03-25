/*
 * test_undo_alter_index_add_err.cpp
 *
 *  Created on: 2023/07/24
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "../../toolkit_alter/TestDbSchemaAlter01.h"

#include "engine/compiler/SmartContractParser.h"

#include "engine/sc_analyze/AnalyzeContext.h"

#include "alinous_lang/AlinousLang.h"
#include "snapshot/FileStoreSnapshot.h"

#include "engine/CodableDatabase.h"

#include "trx/session/base/CdbDatabaseSessionId.h"

#include "snapshot/SnapShotSha256.h"
using namespace codablecash;

TEST_GROUP(TestUndoAlterIndexAddGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};


TEST(TestUndoAlterIndexAddGroup, case01){
	File testCaseFolder = this->env->testCaseDir();

	TestDbSchemaAlter01 tester(this->env);
	tester.init(1024*10);
	tester.insert01();

	CodableDatabase* db = tester.getDatabase();

	File* snapshotFile = testCaseFolder.get(L"snap.bin"); __STP(snapshotFile);
	File* snapshotFile2 = testCaseFolder.get(L"snap2.bin"); __STP(snapshotFile2);

	VirtualMachine* vm = tester.getVm();

	{
		FileStoreSnapshot handler;
		handler.openOutFile(snapshotFile);
		db->snapshot(&handler);

		handler.close();
	}

	CdbDatabaseSessionId* sessionId = CdbDatabaseSessionId::createRandomId(); __STP(sessionId);
	db->newSession(sessionId);

	{
		const File* projectFolder = this->env->getProjectRoot();
		_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_index/resources/exec_index/addIndex01.alns"))
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

