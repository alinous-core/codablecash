/*
 * TestDbSchemaAlterTextUnique01.cpp
 *
 *  Created on: 2020/10/02
 *      Author: iizuka
 */

#include "TestDbSchemaAlterTextUnique01.h"

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

TestDbSchemaAlterTextUnique01::TestDbSchemaAlterTextUnique01(TestEnv* env) : TestDbSchemaBase(env) {

}

TestDbSchemaAlterTextUnique01::~TestDbSchemaAlterTextUnique01() {

}

void TestDbSchemaAlterTextUnique01::init(uint64_t memCapacity) {
	TestDbSchemaBase::init(memCapacity);
	createTable();
}

void TestDbSchemaAlterTextUnique01::createTable() {
	CdbTransaction* trx = getDatabase()->newTransaction(); __STP(trx);

	CreateTableLog* cmd = new CreateTableLog(1L);
	CdbTable* table = new CdbTable(0);

	UnicodeString* testSchema = new UnicodeString(L"public");
	table->setSchemaName(testSchema);
	table->setName(new UnicodeString(L"test_table"));

	table->addColumn(10, L"id", AbstractCdbValue::TYPE_INT, 0, false, false, nullptr);
	table->addColumn(11, L"name", AbstractCdbValue::TYPE_STRING, 8, false, true, L"");
	table->addColumn(12, L"email_id", AbstractCdbValue::TYPE_INT, 0, false, false, L"0");

	table->setPrimaryKey(L"id");

	cmd->setTable(table);

	trx->createTable(cmd);
	trx->commit();
}

void TestDbSchemaAlterTextUnique01::insert01() {
	CdbTransaction* trx = getDatabase()->newTransaction(); __STP(trx);

	insertRecord(trx, 1, L"tanaka", 11);
	insertRecord(trx, 2, L"sato", 12);
	insertRecord(trx, 3, nullptr, 13);
	insertRecord(trx, 4, L"100", 14);

	trx->commit();
}

void TestDbSchemaAlterTextUnique01::insert02() {
	CdbTransaction* trx = getDatabase()->newTransaction(); __STP(trx);

	insertRecord(trx, 1, L"tanaka", 11);
	insertRecord(trx, 2, L"sato", 12);
	insertRecord(trx, 3, L"saito", 13);

	trx->commit();
}

void TestDbSchemaAlterTextUnique01::insert03() {
	CdbTransaction* trx = getDatabase()->newTransaction(); __STP(trx);

	insertRecord(trx, 1, L"tanaka", 11);
	insertRecord(trx, 2, L"sato", 12);
	insertRecord(trx, 3, L"saito", 13);
	insertRecord(trx, 4, nullptr, 14);

	trx->commit();
}

void TestDbSchemaAlterTextUnique01::insertRecord(CdbTransaction* trx, int id,
		const wchar_t* name, int email_id) {
	InsertLog* log = new InsertLog(1L);

	CdbTableIdentifier* tableId = new CdbTableIdentifier();
	tableId->setTable(new UnicodeString(L"test_table"));
	log->setTable(tableId);

	CdbRecord* record = new CdbRecord();
	LocalCdbOid loid(this->loidSerial++);
	record->setOid(&loid);

	record->addValue(new CdbIntValue(id));

	if(name != nullptr){
		record->addValue(new CdbStringValue(name));
	}
	else{
		record->addValue(nullptr);
	}

	record->addValue(new CdbIntValue(email_id));

	log->addRecord(record);

	trx->insert(log);
}

} /* namespace codablecash */
