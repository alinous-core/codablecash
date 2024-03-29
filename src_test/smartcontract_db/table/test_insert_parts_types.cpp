/*
 * test_insert_parts_types.cpp
 *
 *  Created on: 2020/08/06
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base/StackRelease.h"
#include "base_io/File.h"
#include "base/UnicodeString.h"

#include "trx/transaction_log/CreateTableLog.h"
#include "trx/transaction_log/InsertLog.h"
#include "trx/transaction_log/TransactionLogFactory.h"
#include "trx/transaction_log/AbstractTransactionLog.h"

#include "trx/transaction/CdbTransaction.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/CdbTableColumn.h"

#include "engine/CodableDatabase.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"

#include "schema_table/record/table_record/CdbTableIdentifier.h"
#include "schema_table/record/table_record/CdbRecord.h"
#include "schema_table/record/table_record_value/CdbIntValue.h"
#include "schema_table/record/table_record_value/CdbStringValue.h"
#include "schema_table/record/table_record_value/CdbByteValue.h"
#include "schema_table/record/table_record_value/CdbShortValue.h"
#include "schema_table/record/table_record_value/CdbLongValue.h"
#include "schema_table/record/table_record/CdbKeyFactory.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"
#include "schema_table/record/table_record_key/CdbByteKey.h"
#include "schema_table/record/table_record_key/CdbLongKey.h"

#include "schema_table/record/table_record_key/CdbShortKey.h"

#include "schema_table/record/table_record_value/CdbOidValueList.h"

#include "engine/CdbOid.h"

#include "schema_table/record/table_record_value/VmInstanceValueConverter.h"

#include "schema_table/record/table_record_local/LocalCdbOid.h"

using namespace alinous;
using namespace codablecash;

TEST_GROUP(TestInsertPartTypesGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

static void initDb(CodableDatabase& db, File* dbDir, File* undoDir) {
	db.createDatabase(dbDir, undoDir);
	db.loadDatabase(dbDir, undoDir);

	CdbTransaction* trx = db.newTransaction(); __STP(trx);

	CreateTableLog* cmd = new CreateTableLog(1L);
	CdbTable* table = new CdbTable(0);

	UnicodeString* testSchema = new UnicodeString(L"public");
	table->setSchemaName(testSchema);
	table->setName(new UnicodeString(L"test_table"));

	table->addColumn(10, L"id", AbstractCdbValue::TYPE_INT, 0, true, true, nullptr);
	table->addColumn(11, L"name", AbstractCdbValue::TYPE_STRING, 0, true, true, L"");
	table->addColumn(12, L"bvalue", AbstractCdbValue::TYPE_BYTE, 0, true, true, nullptr);
	table->addColumn(13, L"svalue", AbstractCdbValue::TYPE_SHORT, 0, true, true, nullptr);
	table->addColumn(14, L"lvalue", AbstractCdbValue::TYPE_LONG, 0, true, true, nullptr);

	table->setPrimaryKey(L"id");

	cmd->setTable(table);

	trx->createTable(cmd);
	trx->commit();
}

TEST(TestInsertPartTypesGroup, case01){
	File testCaseFolder = this->env->testCaseDir();
	File* dbDir = testCaseFolder.get(L"db"); __STP(dbDir);
	File* undoDir = testCaseFolder.get(L"undo"); __STP(undoDir);
	CodableDatabase db;

	initDb(db, dbDir, undoDir);

	InsertLog* log = new InsertLog(1L);

	CdbTableIdentifier* tableId = new CdbTableIdentifier();
	tableId->setTable(new UnicodeString(L"test_table"));
	log->setTable(tableId);

	CdbRecord* record = new CdbRecord();
	LocalCdbOid loid(1);
	record->setOid(&loid);

	record->addValue(new CdbIntValue(1));
	record->addValue(new CdbStringValue(L"hello"));
	record->addValue(new CdbByteValue(2));
	record->addValue(new CdbShortValue(3));
	record->addValue(new CdbLongValue(4));

	log->addRecord(record);

	CdbTransaction* trx = db.newTransaction(); __STP(trx);
	trx->insert(log);
	trx->commit();
}

TEST(TestInsertPartTypesGroup, factory01){
	CdbKeyFactory factory;

	AbstractBtreeKey* key = factory.fromBinary(0, nullptr);
	CHECK(key == nullptr);
}

TEST(TestInsertPartTypesGroup, toKey01){
	CdbByteValue val(1);
	AbstractCdbKey* k = val.toKey(); __STP(k);

	CdbByteKey* key = dynamic_cast<CdbByteKey*>(k);
	CHECK(key->getValue() == 1);
}

TEST(TestInsertPartTypesGroup, toKey02){
	CdbShortValue val(1);
	AbstractCdbKey* k = val.toKey(); __STP(k);

	CdbShortKey* key = dynamic_cast<CdbShortKey*>(k);
	CHECK(key->getValue() == 1);
}

TEST(TestInsertPartTypesGroup, toKey03){
	CdbLongValue val(1);
	AbstractCdbKey* k = val.toKey(); __STP(k);

	CdbLongKey* key = dynamic_cast<CdbLongKey*>(k);
	CHECK(key->getValue() == 1);
}

TEST(TestInsertPartTypesGroup, CdbOidValueList01){
	CdbOidValueList list;
	CdbOid* oid = new CdbOid(1);

	list.addOid(oid);

	CdbOidValueList* list2 = dynamic_cast<CdbOidValueList*>(list.copy()); __STP(list2);

	int maxLoop = list.size();
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* o1 = list.get(i);
		CdbOid* o2 = list2->get(i);

		CHECK(o1->equals(o2))
	}
}

TEST(TestInsertPartTypesGroup, toKey04){
	CdbOidValueList val;
	AbstractCdbKey* k = val.toKey();

	CHECK(k == nullptr);
}


