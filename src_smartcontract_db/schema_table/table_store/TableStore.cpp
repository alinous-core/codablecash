/*
 * TableStore.cpp
 *
 *  Created on: 2020/05/16
 *      Author: iizuka
 */

#include "schema_table/table_store/TableStore.h"
#include "schema_table/table_store/RecordStore.h"
#include "schema_table/table_store/IndexStore.h"

#include "base_io/File.h"

#include "schema_table/table/CdbTable.h"

#include "schema_table/schema/Schema.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "engine/CdbOid.h"
#include "engine/CdbException.h"

#include "engine_lock/TableLevelLock.h"
#include "engine_lock/AbstractLockHandle.h"
#include "engine_lock/WriteLockHandle.h"
#include "engine_lock/ReadLockHandle.h"
#include "engine_lock/AbstractLockHandle.h"
#include "engine_lock/AbstractDatabaseLock.h"

#include "schema_table/table/CdbTableIndex.h"
#include "schema_table/table/CdbTableColumn.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"

#include "schema_table/record/table_record_key/CdbLongKey.h"

#include "schema_table/table_store/RecordValueConverter.h"
#include "schema_table/table_store/AlterRecordValueExecutor.h"

#include "btree/Btree.h"
#include "btree/BtreeScanner.h"

#include "schema_table/schema/SchemaManager.h"

#include "schema_table/schema_alter_ctx/ColumnModifyContext.h"
#include "schema_table/schema_alter_ctx/TableRenameContext.h"

#include "schema_table/record/table_record_key/CdbRecordKey.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "schema_table/record/table_record_value/CdbOidValueList.h"

#include "schema_table/table_store/IndexValidationException.h"

#include "trx/session/record/UndoDeletedRecordColumnPart.h"

#include "trx/session/base/CdbDatabaseSession.h"
namespace codablecash {

TableStore::TableStore(DiskCacheManager* cacheManager, const File* baseDir, const CdbTable* table) {
	this->baseDir = new File(*baseDir);
	this->table = table;

	this->recordStore = nullptr;
	this->indexStores = new HashMap<CdbOid, IndexStore>();

	this->cacheManager = cacheManager;
	this->tableLock = new TableLevelLock();
}

TableStore::~TableStore() {
	closeTable();

	delete this->baseDir;
	this->table = nullptr;

	delete this->indexStores;

	this->cacheManager = nullptr;
	delete this->tableLock;
}

void TableStore::cleanTableStore(const CdbTable* table, const TableStore* store) {
	const Schema* schema = table->getSchema();
	const UnicodeString* schemaName = schema->getName();
	const UnicodeString* tableName = table->getName();

	File* schemaDir = store->baseDir->get(schemaName); __STP(schemaDir);
	File* tableDir = schemaDir->get(tableName); __STP(tableDir);

	bool result = tableDir->deleteDir();
	assert(result);
}

void TableStore::closeTable() {
	if(this->recordStore != nullptr){
		this->recordStore->close();
		delete this->recordStore;
		this->recordStore = nullptr;

		Iterator<CdbOid>* it = this->indexStores->keySet()->iterator(); __STP(it);
		while(it->hasNext()){
			const CdbOid* oid = it->next();
			IndexStore* store = this->indexStores->get(oid);

			delete store;
		}
		this->indexStores->clear();
	}

}

void TableStore::loadTable() {
	const Schema* schema = this->table->getSchema();
	const UnicodeString* schemaName = schema->getName();
	const UnicodeString* tableName = this->table->getName();

	File* schemaDir = this->baseDir->get(schemaName); __STP(schemaDir);
	File* tableDir = schemaDir->get(tableName); __STP(tableDir);

	this->recordStore = new RecordStore(this->cacheManager ,tableDir, this->table);

	const ArrayList<CdbTableIndex, CdbTableIndexCompare>* list = this->table->getIndexes();

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		const CdbTableIndex* index = list->get(i);

		IndexStore* store = new IndexStore(this->cacheManager, tableDir, this->table, index);

		this->indexStores->put(store->getIndexOid(), store);
	}

	// load
	this->recordStore->load();

	Iterator<CdbOid>* it = this->indexStores->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const CdbOid* oid = it->next();
		IndexStore* store = this->indexStores->get(oid);
		store->load();
	}
}

const CdbOid* TableStore::getOid() const noexcept {
	return this->table->getOid();
}

const CdbTable* TableStore::getTable() const noexcept {
	return this->table;
}

void TableStore::createTable() {
	const Schema* schema = this->table->getSchema();
	const UnicodeString* schemaName = schema->getName();
	const UnicodeString* tableName = this->table->getName();

	File* schemaDir = this->baseDir->get(schemaName); __STP(schemaDir);
	File* tableDir = schemaDir->get(tableName); __STP(tableDir);

	tableDir->deleteDir();
	tableDir->mkdirs();

	RecordStore::createStore(tableDir, this->table, this->cacheManager);

	const ArrayList<CdbTableIndex, CdbTableIndexCompare>* list = this->table->getIndexes();

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		const CdbTableIndex* index = list->get(i);

		IndexStore::createStore(tableDir, this->table, index, this->cacheManager);
	}
}



void TableStore::addNewColumn(const CdbTableColumn* newColumn) {
	AlterRecordValueExecutor exec(newColumn);

	exec.addColumn(this);
}

void TableStore::removeColumn(const CdbTableColumn* removalColumn) {
	AlterRecordValueExecutor exec(removalColumn);

	exec.removeColumn(this);
}


void TableStore::addNewIndex(const CdbTableIndex* index) {
	const Schema* sc = this->table->getSchema();
	const UnicodeString* schemaName = sc->getName();

	const UnicodeString* tableName = table->getName();

	File* schemaDir = this->baseDir->get(schemaName); __STP(schemaDir);
	File* tableDir = schemaDir->get(tableName); __STP(tableDir);
	IndexStore::createStore(tableDir, this->table, index, this->cacheManager);

	IndexStore* indexstore = new IndexStore(this->cacheManager, tableDir, this->table, index);
	indexstore->load();

	this->indexStores->put(indexstore->getIndexOid(), indexstore);
}

void TableStore::removeIndex(const CdbTableIndex* index) {
	const CdbOid* indexOid = index->getOid();
	IndexStore* indexStore = getIndexStore(indexOid); __STP(indexStore);

	indexStore->close();
	this->indexStores->remove(indexOid);

	const Schema* sc = this->table->getSchema();
	const UnicodeString* schemaName = sc->getName();

	File* schemaDir = this->baseDir->get(schemaName); __STP(schemaDir);

	const UnicodeString* tableName = table->getName();
	File* tableDir = schemaDir->get(tableName); __STP(tableDir);

	IndexStore::cleanupStore(tableDir, this->table, index);
}

void TableStore::resetAllIndexes() {
	Iterator<CdbOid>* it = this->indexStores->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const CdbOid* oid = it->next();
		IndexStore* store = this->indexStores->get(oid);

		store->reset();
	}
}

void TableStore::modifyRecords(CdbTransactionManager* trxManager, const ColumnModifyContext* ctx) {
	CdbDatabaseSession* session = trxManager->getCdbDatabaseSession();

	CdbTableColumn* column = ctx->getColumn();
	const AbstractCdbValue* defaultValue = ctx->getDefaultValue();

	RecordValueConverter converter(column, defaultValue);
	converter.setDatabaseSession(session);

	Btree* btree = this->recordStore->getBtree();
	BtreeScanner* scanner = btree->getScanner(); __STP(scanner);

	scanner->begin();

	while(scanner->hasNext()){
		const IBlockObject* obj = scanner->next();
		const CdbRecord* record = dynamic_cast<const CdbRecord*>(obj);

		// convert value
		CdbRecord* newRecord = converter.processUpdate(record); __STP(newRecord);

		// after resetAllIndexes
		this->recordStore->insert(newRecord);
	}
}

void TableStore::buildAllIndexes() {
	ArrayList<IndexStore> list;
	Iterator<CdbOid>* it = this->indexStores->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const CdbOid* oid = it->next();
		IndexStore* store = this->indexStores->get(oid);

		list.addElement(store);
	}

	Btree* btree = this->recordStore->getBtree();
	BtreeScanner* scanner = btree->getScanner(); __STP(scanner);

	scanner->begin();

	while(scanner->hasNext()){
		const IBlockObject* obj = scanner->next();
		const CdbRecord* record = dynamic_cast<const CdbRecord*>(obj);

		addRecord2Index(&list, record);
	}
}

void TableStore::addUndoDeleteColumnPart(CdbDatabaseSession *session, int pos) const {
	Btree* btree = this->recordStore->getBtree();
	BtreeScanner* scanner = btree->getScanner(); __STP(scanner);

	scanner->begin();

	while(scanner->hasNext()){
		const IBlockObject* obj = scanner->next();
		const CdbRecord* record = dynamic_cast<const CdbRecord*>(obj);

		UndoDeletedRecordColumnPart partLog;

		const CdbOid* roid = record->getOid();
		partLog.setRecordOid(roid->getOidValue());
		partLog.setPosition(pos);
		partLog.setLastValue(record->get(pos));

		session->addUndoLogPart(&partLog);
	}
}


void TableStore::addRecord2Index(const ArrayList<IndexStore>* indexStoreList, const CdbRecord* rec) {
	int maxLoop = indexStoreList->size();
	for(int i = 0; i != maxLoop; ++i){
		IndexStore* store = indexStoreList->get(i);

		store->insert(rec);
	}
}

void TableStore::buildIndex(const CdbTableIndex* index) {
	const CdbOid* indexOid = index->getOid();
	IndexStore* store = getIndexStore(indexOid);

	Btree* btree = this->recordStore->getBtree();
	BtreeScanner* scanner = btree->getScanner(); __STP(scanner);

	scanner->begin();

	while(scanner->hasNext()){
		const IBlockObject* obj = scanner->next();
		const CdbRecord* record = dynamic_cast<const CdbRecord*>(obj);

		store->insert(record);
	}
}

void TableStore::insert(const CdbRecord* rec) {
	this->recordStore->insert(rec);

	addToIndexes(rec);
}

void TableStore::addToIndexes(const CdbRecord* rec) {
	Iterator<CdbOid>* it = this->indexStores->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const CdbOid* oid = it->next();
		IndexStore* store = this->indexStores->get(oid);

		store->insert(rec);
	}
}

void TableStore::remove(const CdbOid* oid) {
	CdbRecord* record = this->recordStore->findRecord(oid); __STP(record);

	removeFromIndexes(record, oid);

	this->recordStore->remove(oid);
}

void TableStore::removeFromIndexes(const CdbRecord* rec, const CdbOid* oid) {
	Iterator<CdbOid>* it = this->indexStores->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const CdbOid* idxoid = it->next();
		IndexStore* store = this->indexStores->get(idxoid);

		store->removeIndex(rec, oid);
	}
}

void TableStore::onRename(SchemaManager* mgr, TableRenameContext* ctx) {
	this->recordStore->close(false);

	const UnicodeString* newName = ctx->getDstTable();
	this->recordStore->onRename(newName);

	closeTable();

	const Schema* schema = this->table->getSchema();
	const UnicodeString* schemaName = schema->getName();
	const UnicodeString* tableName = this->table->getName();

	File* schemaDir = this->baseDir->get(schemaName); __STP(schemaDir);
	File* tableDir = schemaDir->get(tableName); __STP(tableDir);

	const UnicodeString* newSchema = ctx->getDstSchema();
	File* newSchemaDir = this->baseDir->get(newSchema); __STP(newSchemaDir);
	File* newTableDir = newSchemaDir->get(newName); __STP(newTableDir);

	ctx->commitSchemaDir(mgr);

	tableDir->move(newTableDir);

	ctx->commit(mgr);
	loadTable();
}

void TableStore::__testCheckIndexes() {
	Btree* btree = this->recordStore->getBtree();
	BtreeScanner* scanner = btree->getScanner(); __STP(scanner);

	scanner->begin();

	while(scanner->hasNext()){
		const IBlockObject* obj = scanner->next();
		const CdbRecord* record = dynamic_cast<const CdbRecord*>(obj);

		__testCheckRecordIndex(record);
	}
}

void TableStore::__testCheckRecordIndex(const CdbRecord *record) {
	const ArrayList<CdbTableIndex, CdbTableIndexCompare>* list = this->table->getIndexes();
	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		CdbTableIndex* index = list->get(i);

		const CdbOid* indexOid = index->getOid();
		IndexStore* indexStore = getIndexStore(index->getOid());
		CdbRecordKey* key = indexStore->makeKey(record); __STP(key);

		CdbOidValueList* oidlist = indexStore->find(key); __STP(oidlist);
		__checkNotNull(oidlist);

		if(index->isUnique()){
			__checkIsUnique(oidlist);
		}
	}
}

void TableStore::__checkNotNull(CdbOidValueList *oidlist) {
	if(oidlist == nullptr){
		throw new IndexValidationException(__FILE__, __LINE__);
	}
}

void TableStore::__checkIsUnique(CdbOidValueList *oidlist) {
	if(oidlist->size() != 1){
		throw new IndexValidationException(__FILE__, __LINE__);
	}
}

void TableStore::validateRecord(CdbRecord* rec) {
	const ArrayList<CdbTableColumn>* metaData = this->table->getColumns();
	const ArrayList<AbstractCdbValue>* values = rec->getValues();

	int metaSize = metaData->size();
	int valueSize = values->size();

	if(metaSize != valueSize){
		throw new CdbException(L"Record's column size is wrong.", __FILE__, __LINE__);
	}

	// check not null
	for(int i = 0; i != metaSize; ++i){
		CdbTableColumn* meta = metaData->get(i);
		AbstractCdbValue* value = values->get(i);

		validateRecordColumnValue(meta, value);
	}

	// check unique with index
	const ArrayList<CdbTableIndex, CdbTableIndexCompare>* list = this->table->getIndexes();
	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		CdbTableIndex* index = list->get(i);

		if(index->isUniqueDataRequired()){
			IndexStore* indexStore = getIndexStore(index->getOid());
			CdbRecordKey* key = indexStore->makeKey(rec); __STP(key);

			const CdbOid* recordOid = rec->getOid();

			if(indexStore->hasKey(key, recordOid)){
				throw new CdbException(L"Record's column must not be unique.", __FILE__, __LINE__);
			}
		}
	}
}

void TableStore::validateRecordColumnValue(CdbTableColumn* meta, AbstractCdbValue* value) {
	if(meta->isNotnull() && (value == nullptr || value->isNull())){
		throw new CdbException(L"Record's column must not be null.", __FILE__, __LINE__);
	}
}

IndexStore* TableStore::getIndexStore(const CdbOid* indexoid) const noexcept {
	return this->indexStores->get(indexoid);
}

CdbRecord* TableStore::findRecord(const CdbOid* recordOid) {
	AbstractCdbKey* key = recordOid->toKey(); __STP(key);

	Btree* btree = this->recordStore->getBtree();

	IBlockObject* obj = btree->findByKey(key);
	if(obj == nullptr){
		return nullptr;
	}

	CdbRecord* record = dynamic_cast<CdbRecord*>(obj);
	assert(record != nullptr);

	return record;
}

AbstractLockHandle* TableStore::writeLock() {
	return this->tableLock->writeLock();
}

AbstractLockHandle* TableStore::readLock() {
	return this->tableLock->readLock();
}

} /* namespace codablecash */
