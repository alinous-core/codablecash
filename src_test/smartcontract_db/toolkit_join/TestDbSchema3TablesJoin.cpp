/*
 * TestDbSchema3TableJoin.cpp
 *
 *  Created on: 2022/01/04
 *      Author: iizuka
 */

#include "TestDbSchema3TablesJoin.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "engine/CodableDatabase.h"

#include "trx/transaction/CdbTransaction.h"

#include "trx/transaction_log/CreateTableLog.h"
#include "trx/transaction_log/InsertLog.h"

#include "schema_table/table/CdbTable.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"

#include "schema_table/record/table_record/CdbTableIdentifier.h"
#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record_local/LocalCdbOid.h"

#include "schema_table/record/table_record_value/CdbIntValue.h"
#include "schema_table/record/table_record_value/CdbStringValue.h"

namespace codablecash {

TestDbSchema3TablesJoin::TestDbSchema3TablesJoin(TestEnv* env) : TestDbSchemaBase(env){

}

TestDbSchema3TablesJoin::~TestDbSchema3TablesJoin() {
	this->list1.deleteElements();
	this->list2.deleteElements();
	this->list3.deleteElements();
}

void TestDbSchema3TablesJoin::init() {
	TestDbSchemaBase::init();
}

void TestDbSchema3TablesJoin::init(uint64_t memCapacity) {
	TestDbSchemaBase::init(memCapacity);
	initSchema();
}

void TestDbSchema3TablesJoin::initSchema() {
	createTable();
	insertData();
}


void TestDbSchema3TablesJoin::createTable() {
	createTestTable1();
	createTestTable2();
	createTestTable3();
}

void TestDbSchema3TablesJoin::createTestTable1() {
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

void TestDbSchema3TablesJoin::createTestTable2() {
	CdbTransaction* trx = getDatabase()->newTransaction(); __STP(trx);

	CreateTableLog* cmd = new CreateTableLog(1L);
	CdbTable* table = new CdbTable(0);

	UnicodeString* testSchema = new UnicodeString(L"public");
	table->setSchemaName(testSchema);
	table->setName(new UnicodeString(L"emails"));

	table->addColumn(20, L"email_id", AbstractCdbValue::TYPE_INT, 0, true, false, nullptr);
	table->addColumn(21, L"email", AbstractCdbValue::TYPE_STRING, 0, true, false, L"");
	table->addColumn(22, L"server_id", AbstractCdbValue::TYPE_INT, 0, true, false, nullptr);

	ArrayList<UnicodeString> keys;
	keys.addElement(new UnicodeString(L"email_id"));
	keys.addElement(new UnicodeString(L"email"));
	keys.setDeleteOnExit();

	table->setPrimaryKeys(&keys);

	cmd->setTable(table);

	trx->createTable(cmd);
	trx->commit();
}

void TestDbSchema3TablesJoin::createTestTable3() {
	CdbTransaction* trx = getDatabase()->newTransaction(); __STP(trx);

	CreateTableLog* cmd = new CreateTableLog(1L);
	CdbTable* table = new CdbTable(0);

	UnicodeString* testSchema = new UnicodeString(L"public");
	table->setSchemaName(testSchema);
	table->setName(new UnicodeString(L"servers"));

	table->addColumn(30, L"server_id", AbstractCdbValue::TYPE_INT, 0, true, false, nullptr);
	table->addColumn(31, L"server", AbstractCdbValue::TYPE_STRING, 0, true, false, L"");

	table->setPrimaryKey(L"server_id");

	cmd->setTable(table);

	trx->createTable(cmd);
	trx->commit();
}

void TestDbSchema3TablesJoin::insertData() {
	insertData1();
	insertData2();
	insertData3();
}

void TestDbSchema3TablesJoin::insertData1() {
	CdbTransaction* trx = getDatabase()->newTransaction(); __STP(trx);

	insertRecord1(trx, 1, L"tanaka", 11, &list1);
	insertRecord1(trx, 2, L"yamada", 12, &list1);
	insertRecord1(trx, 3, L"yamamoto", 13, &list1);
	insertRecord1(trx, 4, L"iizuka", 14, &list1);
	insertRecord1(trx, 5, L"sato", 15, &list1);
	insertRecord1(trx, 6, L"fujita", 16, &list1);
	insertRecord1(trx, 7, L"inoue", 17, &list1);

	trx->commit();
}

void TestDbSchema3TablesJoin::insertRecord1(CdbTransaction* trx, int id,
		const wchar_t* name, int email_id, ArrayList<CdbRecord>* list) {
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

	if(list != nullptr){
		list->addElement((CdbRecord*)record->copy());
	}

	trx->insert(log);
}

void TestDbSchema3TablesJoin::insertData2() {
	CdbTransaction* trx = getDatabase()->newTransaction(); __STP(trx);

	insertRecord2(trx, 11, L"tanaka@tanaka.com", 20, &list2);
	insertRecord2(trx, 11, L"yamada@yamada.com", 21, &list2);

	trx->commit();
}

void TestDbSchema3TablesJoin::insertRecord2(CdbTransaction* trx, int email_id,
		const wchar_t* email, int server_id, ArrayList<CdbRecord>* list) {
	InsertLog* log = new InsertLog(1L);

	CdbTableIdentifier* tableId = new CdbTableIdentifier();
	tableId->setTable(new UnicodeString(L"emails"));
	log->setTable(tableId);

	CdbRecord* record = new CdbRecord();
	LocalCdbOid loid(this->loidSerial++);
	record->setOid(&loid);

	record->addValue(new CdbIntValue(email_id));
	record->addValue(new CdbStringValue(email));
	record->addValue(new CdbIntValue(server_id));

	log->addRecord(record);

	if(list != nullptr){
		list->addElement((CdbRecord*)record->copy());
	}

	trx->insert(log);
}

void TestDbSchema3TablesJoin::insertData3() {
	CdbTransaction* trx = getDatabase()->newTransaction(); __STP(trx);

	insertRecord3(trx, 20, L"tanaka.com", &list3);
	insertRecord3(trx, 21, L"yamada.com", &list3);

	trx->commit();
}

void TestDbSchema3TablesJoin::insertRecord3(CdbTransaction* trx, int server_id,
		const wchar_t* server, ArrayList<CdbRecord>* list) {
	InsertLog* log = new InsertLog(1L);

	CdbTableIdentifier* tableId = new CdbTableIdentifier();
	tableId->setTable(new UnicodeString(L"servers"));
	log->setTable(tableId);

	CdbRecord* record = new CdbRecord();
	LocalCdbOid loid(this->loidSerial++);
	record->setOid(&loid);

	record->addValue(new CdbIntValue(server_id));
	record->addValue(new CdbStringValue(server));

	log->addRecord(record);

	if(list != nullptr){
		list->addElement((CdbRecord*)record->copy());
	}

	trx->insert(log);

}

} /* namespace codablecash */
