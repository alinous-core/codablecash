/*
 * TestDbSchemaAlter02.cpp
 *
 *  Created on: 2020/09/23
 *      Author: iizuka
 */

#include "TestDbSchemaAlter02.h"

#include "schema_table/table/CdbTable.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"

#include "base/StackRelease.h"

#include "engine/CodableDatabase.h"

#include "trx/transaction/CdbTransaction.h"

#include "trx/transaction_log/CreateTableLog.h"
#include "trx/transaction_log/InsertLog.h"

#include "schema_table/record/table_record/CdbRecord.h"
#include "schema_table/record/table_record/CdbTableIdentifier.h"

#include "schema_table/record/table_record_value/CdbIntValue.h"
#include "schema_table/record/table_record_value/CdbStringValue.h"

#include "schema_table/record/table_record_value/VmInstanceValueConverter.h"

#include "schema_table/record/table_record_local/LocalCdbOid.h"
namespace codablecash {

TestDbSchemaAlter02::TestDbSchemaAlter02(TestEnv* env) : TestDbSchemaBase(env) {

}

TestDbSchemaAlter02::~TestDbSchemaAlter02() {

}

void TestDbSchemaAlter02::init(uint64_t memCapacity) {
	TestDbSchemaBase::init(memCapacity);
	createTable();
}

void TestDbSchemaAlter02::createTable() {
	CdbTransaction* trx = getDatabase()->newTransaction(); __STP(trx);

	CreateTableLog* cmd = new CreateTableLog(1L);
	CdbTable* table = new CdbTable(0);

	UnicodeString* testSchema = new UnicodeString(L"public");
	table->setSchemaName(testSchema);
	table->setName(new UnicodeString(L"test_table"));

	table->addColumn(10, L"id", AbstractCdbValue::TYPE_INT, 0, true, true, nullptr);
	table->addColumn(11, L"name", AbstractCdbValue::TYPE_STRING, 0, true, false, L"");
	table->addColumn(12, L"email_id", AbstractCdbValue::TYPE_INT, 0, false, true, L"0");

	table->setPrimaryKey(L"id");

	cmd->setTable(table);

	trx->createTable(cmd);
	trx->commit();
}

void TestDbSchemaAlter02::insert01() {
	CdbTransaction* trx = getDatabase()->newTransaction(); __STP(trx);

	insertRecord(trx, 1, L"tanaka", 11);

	trx->commit();
}

void TestDbSchemaAlter02::insertRecord(CdbTransaction* trx, int id,	const wchar_t* name, int email_id) {
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
