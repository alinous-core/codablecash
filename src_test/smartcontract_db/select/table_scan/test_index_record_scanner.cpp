/*
 * test_index_record_scanner.cpp
 *
 *  Created on: 2020/08/18
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "engine/CodableDatabase.h"
#include "engine/CdbOid.h"

#include "base_io/File.h"

#include "trx/transaction/CdbTransaction.h"

#include "trx/transaction_log/CreateTableLog.h"

#include "schema_table/table/CdbTable.h"

#include "trx/transaction_log/InsertLog.h"

#include "schema_table/record/table_record/CdbTableIdentifier.h"
#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record_value/CdbIntValue.h"
#include "schema_table/record/table_record_value/CdbStringValue.h"

#include "schema_table/record/table_record_key/CdbRecordKey.h"

#include "schema_table/record/table_record_key/CdbIntKey.h"

#include "base/StackRelease.h"

#include "scan_select/scan/IndexRecordScanner.h"

#include "schema_table/table_store/TableStore.h"

#include "schema_table/record/table_record_value/VmInstanceValueConverter.h"

#include "schema_table/record/table_record_local/LocalCdbOid.h"

using namespace codablecash;


static void initDb(CodableDatabase& db, File* dbDir, File* undoDir);
static void insertRecord(CdbTransaction* trx, int id, const wchar_t* name, ArrayList<CdbRecord>* list);

TEST_GROUP(TestIndexRecordScannerGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

void initDb(CodableDatabase& db, File* dbDir, File* undoDir) {
	db.createDatabase(dbDir, undoDir);
	db.loadDatabase(dbDir, undoDir);

	CdbTransaction* trx = db.newTransaction(); __STP(trx);

	CreateTableLog* cmd = new CreateTableLog(1L);
	CdbTable* table = new CdbTable(0);

	UnicodeString* testSchema = new UnicodeString(L"public");
	table->setSchemaName(testSchema);
	table->setName(new UnicodeString(L"test_table"));

	table->addColumn(10, L"id", AbstractCdbValue::TYPE_INT, 0, true, true, nullptr);
	table->addColumn(11, L"name", AbstractCdbValue::TYPE_STRING, 0, true, false, L"");
	table->setPrimaryKey(L"id");

	cmd->setTable(table);

	trx->createTable(cmd);
	trx->commit();
}

void insertRecord(CdbTransaction* trx, int id, const wchar_t* name, ArrayList<CdbRecord>* list) {
	static int serial = 1;

	InsertLog* log = new InsertLog(1L);

	CdbTableIdentifier* tableId = new CdbTableIdentifier();
	tableId->setTable(new UnicodeString(L"test_table"));
	log->setTable(tableId);

	CdbRecord* record = new CdbRecord();
	LocalCdbOid loid(serial++);
	record->setOid(&loid);

	record->addValue(new CdbIntValue(id));

	record->addValue(new CdbStringValue(name));

	log->addRecord(record);

	if(list != nullptr){
		list->addElement((CdbRecord*)record->copy());
	}


	trx->insert(log);
}


TEST(TestIndexRecordScannerGroup, case01){
	File testCaseFolder = this->env->testCaseDir();
	File* dbDir = testCaseFolder.get(L"db"); __STP(dbDir);
	File* undoDir = testCaseFolder.get(L"undo"); __STP(undoDir);
	CodableDatabase db;

	initDb(db, dbDir, undoDir);
	ArrayList<CdbRecord> list; list.setDeleteOnExit();
	{
		CdbTransaction* trx = db.newTransaction(); __STP(trx);
		insertRecord(trx, 1, L"tanaka", &list);
		insertRecord(trx, 2, L"yamada", &list);
		insertRecord(trx, 3, L"yamamoto", &list);
		insertRecord(trx, 4, L"iizuka", &list);
		insertRecord(trx, 5, L"sato", &list);
		insertRecord(trx, 6, L"fujita", &list);
		insertRecord(trx, 7, L"inoue", &list);

		trx->commit();
	}


	{
		ArrayList<CdbRecord> listout; listout.setDeleteOnExit();

		CdbTransaction* trx = db.newTransaction(); __STP(trx);

		UnicodeString colName(L"id");
		CdbTableIdentifier tableId(L"public", L"test_table");

		CdbRecordKey* begin = new CdbRecordKey(); __STP(begin);
		begin->addKey(new CdbIntKey(3));
		CdbRecordKey* end = new CdbRecordKey(); __STP(end);
		end->addKey(new CdbIntKey(6));

		IndexRecordScanner* scanner = trx->getIndexRecordScanner(&tableId, &colName, begin, true, end, true); __STP(scanner);
		scanner->start();

		while(scanner->hasNext()){
			const CdbRecord* record = scanner->next();

			listout.addElement(dynamic_cast<CdbRecord*>(record->copy()));
		}

		int maxLoop = listout.size();
		CHECK(maxLoop == 4);

		int offset = 2;
		for(int i = 0; i != maxLoop; ++i){
			const CdbRecord* record = listout.get(i);
			const CdbRecord* lastrecord = list.get(i + offset);

			AbstractCdbKey* lkey = record->toKey(); __STP(lkey);
			AbstractCdbKey* rkey = lastrecord->toKey(); __STP(rkey);

			int diff = lkey->compareTo(rkey);
			CHECK(diff == 0);
		}
	}
}

TEST(TestIndexRecordScannerGroup, findnull){
	File testCaseFolder = this->env->testCaseDir();
	File* dbDir = testCaseFolder.get(L"db"); __STP(dbDir);
	File* undoDir = testCaseFolder.get(L"undo"); __STP(undoDir);
	CodableDatabase db;

	initDb(db, dbDir, undoDir);
	ArrayList<CdbRecord> list; list.setDeleteOnExit();
	{
		CdbTransaction* trx = db.newTransaction(); __STP(trx);
		insertRecord(trx, 1, L"tanaka", &list);
		insertRecord(trx, 2, L"yamada", &list);
		insertRecord(trx, 3, L"yamamoto", &list);
		insertRecord(trx, 4, L"iizuka", &list);
		insertRecord(trx, 5, L"sato", &list);
		insertRecord(trx, 6, L"fujita", &list);
		insertRecord(trx, 7, L"inoue", &list);

		trx->commit();
	}

	{
		CdbTransaction* trx = db.newTransaction(); __STP(trx);

		UnicodeString colName(L"id");
		CdbTableIdentifier tableId(L"public", L"test_table");

		CdbRecordKey* begin = new CdbRecordKey(); __STP(begin);
		begin->addKey(new CdbIntKey(3));
		CdbRecordKey* end = new CdbRecordKey(); __STP(end);
		end->addKey(new CdbIntKey(6));

		IndexRecordScanner* scanner = trx->getIndexRecordScanner(&tableId, &colName, begin, true, end, true); __STP(scanner);

		TableStore* store = scanner->getTableStore();

		CdbOid tmoOid(10000);
		CdbRecord* record = store->findRecord(&tmoOid);

		CHECK(record == nullptr);
	}
}
