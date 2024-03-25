/*
 * TestDbSchemaOrJoin01.cpp
 *
 *  Created on: 2022/01/18
 *      Author: iizuka
 */

#include "TestDbSchemaOrJoin01.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "trx/transaction/CdbTransaction.h"

#include "engine/CodableDatabase.h"

#include "trx/transaction_log/CreateTableLog.h"
#include "trx/transaction_log/InsertLog.h"

#include "schema_table/table/CdbTable.h"

#include "schema_table/record/table_record/CdbTableIdentifier.h"
#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"
#include "schema_table/record/table_record_value/CdbIntValue.h"
#include "schema_table/record/table_record_value/CdbStringValue.h"

#include "schema_table/record/table_record_local/LocalCdbOid.h"


namespace codablecash {

TestDbSchemaOrJoin01::TestDbSchemaOrJoin01(TestEnv* env) : TestDbSchemaBase(env) {

}

TestDbSchemaOrJoin01::~TestDbSchemaOrJoin01() {
	this->list1.deleteElements();
	this->list2.deleteElements();
}

void TestDbSchemaOrJoin01::init() {
	TestDbSchemaBase::init();
}

void TestDbSchemaOrJoin01::init(uint64_t memCapacity) {
	TestDbSchemaBase::init(memCapacity);

	createTable();
	insertData();
}

void TestDbSchemaOrJoin01::createTable() {
	createTestTable1();
	createTestTable2();
}

void TestDbSchemaOrJoin01::createTestTable1() {
	CdbTransaction* trx = getDatabase()->newTransaction(); __STP(trx);

	CreateTableLog* cmd = new CreateTableLog(1L);
	CdbTable* table = new CdbTable(0);

	UnicodeString* testSchema = new UnicodeString(L"public");
	table->setSchemaName(testSchema);
	table->setName(new UnicodeString(L"test_table"));

	table->addColumn(10, L"id", AbstractCdbValue::TYPE_INT, 0, true, true, nullptr);
	table->addColumn(11, L"name", AbstractCdbValue::TYPE_STRING, 0, true, false, L"");
	table->addColumn(12, L"email_id", AbstractCdbValue::TYPE_INT, 0, true, false, nullptr);
	table->addColumn(13, L"email_id2", AbstractCdbValue::TYPE_INT, 0, true, false, nullptr);

	table->setPrimaryKey(L"id");

	cmd->setTable(table);

	trx->createTable(cmd);
	trx->commit();
}

void TestDbSchemaOrJoin01::createTestTable2() {
	CdbTransaction* trx = getDatabase()->newTransaction(); __STP(trx);

	CreateTableLog* cmd = new CreateTableLog(1L);
	CdbTable* table = new CdbTable(0);

	UnicodeString* testSchema = new UnicodeString(L"public");
	table->setSchemaName(testSchema);
	table->setName(new UnicodeString(L"emails"));

	table->addColumn(10, L"email_id", AbstractCdbValue::TYPE_INT, 0, true, false, nullptr);
	table->addColumn(11, L"email", AbstractCdbValue::TYPE_STRING, 0, true, false, L"");
	table->addColumn(12, L"email_id2", AbstractCdbValue::TYPE_INT, 0, true, false, nullptr);

	ArrayList<UnicodeString> keys;
	keys.addElement(new UnicodeString(L"email_id"));
	keys.addElement(new UnicodeString(L"email"));
	keys.setDeleteOnExit();

	table->setPrimaryKeys(&keys);

	cmd->setTable(table);

	trx->createTable(cmd);
	trx->commit();
}

void TestDbSchemaOrJoin01::insertData() {
	insertData1();
	insertData2();
}

void TestDbSchemaOrJoin01::insertData1() {
	CdbTransaction* trx = getDatabase()->newTransaction(); __STP(trx);
	insertRecord1(trx, 1, L"tanaka", 11, 111, &list1);
	insertRecord1(trx, 2, L"yamada", 12, 112, &list1);
	insertRecord1(trx, 3, L"yamamoto", 13, 113, &list1);
	insertRecord1(trx, 4, L"iizuka", 14, 114, &list1);
	insertRecord1(trx, 5, L"sato", 15, 114, &list1);
	insertRecord1(trx, 6, L"fujita", 16, 116, &list1);
	insertRecord1(trx, 7, L"inoue", 17, 117, &list1);

	trx->commit();
}

void TestDbSchemaOrJoin01::insertRecord1(CdbTransaction* trx, int id,
		const wchar_t* name, int email_id, int email_id2, ArrayList<CdbRecord>* list) {
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
	record->addValue(new CdbIntValue(email_id2));

	log->addRecord(record);

	if(list != nullptr){
		list->addElement((CdbRecord*)record->copy());
	}

	trx->insert(log);

}

void TestDbSchemaOrJoin01::insertData2() {
	CdbTransaction* trx = getDatabase()->newTransaction(); __STP(trx);

	insertRecord2(trx, 11, L"tanaka@tanaka.com", 11, &list2);
	insertRecord2(trx, 11, L"yamada@yamada.com", 11, &list2);

	//insertRecord2(trx, 20, L"nobody@nobody.com", &list2);

	trx->commit();
}

void TestDbSchemaOrJoin01::insertRecord2(CdbTransaction* trx, int email_id,
		const wchar_t* email, int email_id2, ArrayList<CdbRecord>* list) {
	InsertLog* log = new InsertLog(1L);

	CdbTableIdentifier* tableId = new CdbTableIdentifier();
	tableId->setTable(new UnicodeString(L"emails"));
	log->setTable(tableId);

	CdbRecord* record = new CdbRecord();
	LocalCdbOid loid(this->loidSerial++);
	record->setOid(&loid);

	record->addValue(new CdbIntValue(email_id));
	record->addValue(new CdbStringValue(email));
	record->addValue(new CdbIntValue(email_id2));

	log->addRecord(record);

	if(list != nullptr){
		list->addElement((CdbRecord*)record->copy());
	}

	trx->insert(log);
}

} /* namespace codablecash */
