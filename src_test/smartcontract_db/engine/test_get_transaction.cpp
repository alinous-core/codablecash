/*
 * test_get_transaction.cpp
 *
 *  Created on: 2020/05/12
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base_io/File.h"
#include "base/StackRelease.h"

#include "engine/CodableDatabase.h"

#include "trx/transaction/CdbTransaction.h"

#include "engine/CdbException.h"
using namespace codablecash;
using namespace alinous;


TEST_GROUP(TestGetTransactionGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};

TEST(TestGetTransactionGroup, case01){
	File testCaseFolder = this->env->testCaseDir();
	File* dbDir = testCaseFolder.get(L"db"); __STP(dbDir);
	File* undoDir = testCaseFolder.get(L"undo"); __STP(undoDir);

	CodableDatabase db;

	db.createDatabase(dbDir, undoDir);
	db.loadDatabase(dbDir, undoDir);

	CdbTransaction* trx = db.newTransaction(); __STP(trx);
	CHECK(trx != nullptr);
}

TEST(TestGetTransactionGroup, case02_err){
	File testCaseFolder = this->env->testCaseDir();
	File* dbDir = testCaseFolder.get(L"db"); __STP(dbDir);
	File* undoDir = testCaseFolder.get(L"undo"); __STP(undoDir);
	CodableDatabase db;

	db.createDatabase(dbDir, undoDir);

	CdbException* ex = nullptr;
	try{
		CdbTransaction* trx = db.newTransaction();
	}catch(CdbException* e){
		ex = e;
	}

	CHECK(ex != nullptr)
	delete ex;
}

TEST(TestGetTransactionGroup, case03){
	File testCaseFolder = this->env->testCaseDir();
	File* dbDir = testCaseFolder.get(L"db"); __STP(dbDir);
	File* undoDir = testCaseFolder.get(L"undo"); __STP(undoDir);
	CodableDatabase db;

	db.createDatabase(dbDir, undoDir);
	db.loadDatabase(dbDir, undoDir);

	CdbTransaction* trx = db.newTransaction(); __STP(trx);
}

