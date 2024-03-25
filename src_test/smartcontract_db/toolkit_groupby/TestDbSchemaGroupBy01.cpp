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
#include "schema_table/record/table_record_value/CdbIntValue.h"
#include "schema_table/record/table_record_value/CdbStringValue.h"

#include "schema_table/record/table_record_local/LocalCdbOid.h"

#include "schema_table/record/table_record/CdbTableIdentifier.h"
#include "schema_table/record/table_record/CdbRecord.h"

#include "trx/transaction/CdbTransaction.h"

#include "trx/transaction_log/CreateTableLog.h"
#include "trx/transaction_log/InsertLog.h"


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

void TestDbSchemaGroupBy01::insertData() {
	CdbTransaction* trx = getDatabase()->newTransaction(); __STP(trx);
	insertRecord1(trx, 1, L"tanaka", 11);
	insertRecord1(trx, 2, L"yamada", 12);
	insertRecord1(trx, 3, L"yamamoto", 13);
	insertRecord1(trx, 4, L"iizuka", 13);
	insertRecord1(trx, 5, L"sato", 11);
	insertRecord1(trx, 6, L"fujita", 13);
	insertRecord1(trx, 7, L"inoue", 11);

	trx->commit();
}

void TestDbSchemaGroupBy01::insertRecord1(CdbTransaction* trx, int id, const wchar_t* name, int email_id) {
	InsertLog* log = new InsertLog(1L);

	CdbTableIdentifier* tableId = new CdbTableIdentifier();
	tableId->setTable(new UnicodeString(L"test_table"));
	log->setTable(tableId);

	CdbRecord* record = new CdbRecord();
	LocalCdbOid loid(this->loidSerial++);
	record->setOid(&loid);

	record->addValue(new CdbIntValue(id));

	record->addValue(new CdbStringValue(name));
	record->addValue(new CdbIntValue(email_id));

	log->addRecord(record);

	trx->insert(log);
}

} /* namespace codablecash */
