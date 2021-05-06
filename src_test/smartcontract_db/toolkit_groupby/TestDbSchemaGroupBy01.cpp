/*
 * TestDbSchemaGroupBy01.cpp
 *
 *  Created on: 2021/05/06
 *      Author: iizuka
 */

#include "TestDbSchemaGroupBy01.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "engine/CodableDatabase.h"

#include "schema_table/table/CdbTable.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"

#include "trx/transaction/CdbTransaction.h"

#include "trx/transaction_log/CreateTableLog.h"


namespace codablecash {

TestDbSchemaGroupBy01::TestDbSchemaGroupBy01(TestEnv* env) : TestDbSchemaBase(env) {

}

TestDbSchemaGroupBy01::~TestDbSchemaGroupBy01() {

}

void TestDbSchemaGroupBy01::init() {
	TestDbSchemaBase::init();
}

void TestDbSchemaGroupBy01::init(uint64_t memCapacity) {
	TestDbSchemaBase::init(memCapacity);

	createTable();
	insertData();
}

void TestDbSchemaGroupBy01::createTable() {
	CdbTransaction* trx = getDatabase()->newTransaction(); __STP(trx);

	CreateTableLog* cmd = new CreateTableLog();
	CdbTable* table = new CdbTable(0);

	UnicodeString* testSchema = new UnicodeString(L"public");
	table->setSchemaName(testSchema);
	table->setName(new UnicodeString(L"test_table"));

	table->addColumn(0, L"id", AbstractCdbValue::TYPE_INT, 0, true, true, nullptr);
	table->addColumn(0, L"name", AbstractCdbValue::TYPE_STRING, 0, true, false, L"");
	table->addColumn(0, L"email_id", AbstractCdbValue::TYPE_INT, 0, true, false, nullptr);

	table->setPrimaryKey(L"id");

	cmd->setTable(table);

	trx->createTable(cmd);
	trx->commit();
}

void TestDbSchemaGroupBy01::insertData() {
}

} /* namespace codablecash */
