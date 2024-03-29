/*
 * test_sql_engine.cpp
 *
 *  Created on: 2020/05/10
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base_io/File.h"
#include "base/StackRelease.h"

#include "engine/CodableDatabase.h"

#include "engine/CdbException.h"

#include "engine/CdbOid.h"
using namespace codablecash;


TEST_GROUP(TestDbEngineGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};

TEST(TestDbEngineGroup, construct){
	File projectFolder = this->env->testCaseDir();
	CodableDatabase db;
}

TEST(TestDbEngineGroup, createDb01){
	File testCaseFolder = this->env->testCaseDir();
	File* dbDir = testCaseFolder.get(L"db"); __STP(dbDir);
	File* undoDir = testCaseFolder.get(L"undo"); __STP(undoDir);
	CodableDatabase db;

	db.createDatabase(dbDir, undoDir);
}

TEST(TestDbEngineGroup, createDb02){
	File testCaseFolder = this->env->testCaseDir();
	File* dbDir = testCaseFolder.get(L"db"); __STP(dbDir);
	File* undoDir = testCaseFolder.get(L"undo"); __STP(undoDir);
	CodableDatabase db;

	db.createDatabase(dbDir, undoDir);

	db.loadDatabase(dbDir, undoDir);
}

TEST(TestDbEngineGroup, createDbLoadError01){
	File testCaseFolder = this->env->testCaseDir();
	File* dbDir = testCaseFolder.get(L"db"); __STP(dbDir);
	File* undoDir = testCaseFolder.get(L"undo"); __STP(undoDir);
	CodableDatabase db;

	bool result = db.loadDatabase(dbDir, undoDir);
	CHECK(!result)
}

TEST(TestDbEngineGroup, testEx){
	testException<CdbException>();
}

TEST(TestDbEngineGroup, testOid01){
	CdbOid oid1(1);
	CdbOid oid2(2);
	CdbOid oid3(1);

	CHECK(!oid1.equals(&oid2));
	CHECK(oid1.equals(&oid3))
}

TEST(TestDbEngineGroup, testOid02){
	CdbOid oid1(1);

	IBlockObject* bobj = oid1.copyData(); __STP(bobj);
	CdbOid* oid2 = dynamic_cast<CdbOid*>(bobj);

	CHECK(oid1.equals(oid2))
}
