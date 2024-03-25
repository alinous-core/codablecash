/*
 * test_undo_drop_table.cpp
 *
 *  Created on: 2023/07/14
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "../../toolkit/TestDbSchema01.h"
#include "engine/compiler/SmartContractParser.h"

#include "../../../../src_gen/alinous_lang/AlinousLang.h"
#include "lang_sql/sql_ddl/DropTableStatement.h"

#include "engine/sc_analyze/AnalyzeContext.h"

#include "engine/CodableDatabase.h"

#include "trx/session/base/CdbDatabaseSessionId.h"

#include "snapshot/SnapShotSha256.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "trx/session/base/CdbDatabaseSession.h"

#include "trx/session/base/UndoRegisterTransaction.h"
using namespace codablecash;

TEST_GROUP(TestUnodoDropTableGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestUnodoDropTableGroup, case01){
	File testCaseFolder = this->env->testCaseDir();
	File* snapshotFile = testCaseFolder.get(L"snap.bin"); __STP(snapshotFile);
	File* snapshotFile2 = testCaseFolder.get(L"snap2.bin"); __STP(snapshotFile2);

	TestDbSchema01 tester(this->env);
	tester.init(1024*10);

	tester.snapshot(snapshotFile);

	CodableDatabase* db = tester.getDatabase();
	CdbDatabaseSessionId* sessionId = CdbDatabaseSessionId::createRandomId(); __STP(sessionId);
	db->newSession(sessionId);

	VirtualMachine* vm = tester.getVm();

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_ddl/resources/droptable/case01.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		DropTableStatement* stmt = lang->dropTableStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		stmt->preAnalyze(actx);
		stmt->analyzeTypeRef(actx);
		stmt->analyze(actx);

		stmt->interpret(vm);

		db->undoSession(sessionId);
	}

	tester.snapshot(snapshotFile2);

	SnapShotSha256 sha1(snapshotFile);
	SnapShotSha256 sha2(snapshotFile2);

	sha1.process();
	sha2.process();

	bool bl = sha1.equals(&sha2);
	CHECK(bl == true);
}

TEST(TestUnodoDropTableGroup, caseRollBack01){
	TestDbSchema01 tester(this->env);
	tester.init(1024*10);

	CodableDatabase* db = tester.getDatabase();
	CdbDatabaseSessionId* sessionId = CdbDatabaseSessionId::createRandomId(); __STP(sessionId);
	db->newSession(sessionId);

	VirtualMachine* vm = tester.getVm();

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_ddl/resources/droptable/case01.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		DropTableStatement* stmt = lang->dropTableStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		stmt->preAnalyze(actx);
		stmt->analyzeTypeRef(actx);
		stmt->analyze(actx);

		stmt->interpret(vm);

		CdbTransactionManager* trxManager = db->getTransactionxManager();
		CdbDatabaseSession* session = trxManager->getCdbDatabaseSession();

		UndoSpaceManager* undoManager = session->getUndoSpaceManager();
		{
			UndoRegisterTransaction undoTrx(undoManager);
			undoTrx.logAdded();
		}

	}

}
