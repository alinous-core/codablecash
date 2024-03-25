/*
 * TestDbSchemaJoin01.cpp
 *
 *  Created on: 2020/12/17
 *      Author: iizuka
 */

#include "TestDbSchemaJoin01.h"

#include "trx/transaction/CdbTransaction.h"

#include "trx/transaction_log/CreateTableLog.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "engine/CodableDatabase.h"

#include "schema_table/table/CdbTable.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"


namespace codablecash {

TestDbSchemaJoin01::TestDbSchemaJoin01(TestEnv* env) : TestDbSchemaBase(env) {

}

TestDbSchemaJoin01::~TestDbSchemaJoin01() {

}

void TestDbSchemaJoin01::init(uint64_t memCapacity) {
	TestDbSchemaBase::init(memCapacity);
	createTable();
}

void TestDbSchemaJoin01::createTable() {
	createTestTable1();
	createTestTable2();
}

void TestDbSchemaJoin01::createTestTable1() {
	CdbTransaction* trx = getDatabase()->newTransaction(); __STP(trx);

	CreateTableLog* cmd = new CreateTableLog(1L);
	CdbTable* table = new CdbTable(0);

	UnicodeString* testSchema = new UnicodeString(L"public");
	table->setSchemaName(testSchema);
	table->setName(new UnicodeString(L"test_table"));

	table->addColumn(10, L"id", AbstractCdbValue::TYPE_INT, 0, true, true, nullptr);
	table->addColumn(11, L"name", AbstractCdbValue::TYPE_STRING, 0, true, false, L"");
	table->addColumn(12, L"email_id", AbstractCdbValue::TYPE_INT, 0, true, false, nullptr);

	table->setPrimaryKey(L"id");

	cmd->setTable(table);

	trx->createTable(cmd);
	trx->commit();
}

void TestDbSchemaJoin01::createTestTable2() {
	CdbTransaction* trx = getDatabase()->newTransaction(); __STP(trx);

	CreateTableLog* cmd = new CreateTableLog(1L);
	CdbTable* table = new CdbTable(0);

	UnicodeString* testSchema = new UnicodeString(L"public");
	table->setSchemaName(testSchema);
	table->setName(new UnicodeString(L"emails"));

	table->addColumn(20, L"email_id", AbstractCdbValue::TYPE_INT, 0, true, true, nullptr);
	table->addColumn(21, L"email", AbstractCdbValue::TYPE_STRING, 0, true, false, L"");

	ArrayList<UnicodeString> keys;
	keys.addElement(new UnicodeString(L"email_id"));
	keys.addElement(new UnicodeString(L"email"));
	keys.setDeleteOnExit();

	table->setPrimaryKeys(&keys);

	cmd->setTable(table);

	trx->createTable(cmd);
	trx->commit();
}

} /* namespace codablecash */
