/*
 * CdbStorageManager.cpp
 *
 *  Created on: 2020/05/16
 *      Author: iizuka
 */

#include "schema_table/table_store/CdbStorageManager.h"
#include "schema_table/table_store/TableStore.h"

#include "schema_table/schema/SchemaManager.h"

#include "random_access_file/DiskCacheManager.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "schema_table/table/CdbTable.h"

#include "engine/CdbOid.h"

#include "schema_table/record/table_record/CdbDataFactory.h"
#include "schema_table/record/table_record/CdbKeyFactory.h"

#include "schema_table/schema/Schema.h"
#include "schema_table/schema/SchemaCompare.h"

#include "schema_table/schema_alter_ctx/ColumnModifyContext.h"
#include "schema_table/schema_alter_ctx/TableRenameContext.h"


namespace codablecash {

CdbKeyFactory CdbStorageManager::keyFactory;
CdbDataFactory CdbStorageManager::dataFactory;

CdbStorageManager::CdbStorageManager() {
	this->schemaManager = nullptr;
	this->cacheManager = new DiskCacheManager();
	this->tableStoreMap = new HashMap<CdbOid, TableStore>();
}

CdbStorageManager::~CdbStorageManager() {
	close();

	delete this->tableStoreMap;
	delete this->cacheManager;
}

void CdbStorageManager::close() {
	if(this->schemaManager != nullptr || !this->tableStoreMap->isEmpty()){
		this->schemaManager = nullptr;

		Iterator<CdbOid> *it = this->tableStoreMap->keySet()->iterator(); __STP(it);
		while(it->hasNext()){
			const CdbOid* oid = it->next();
			TableStore* tableStore = this->tableStoreMap->get(oid);

			delete tableStore;
		}

		this->tableStoreMap->clear();
	}
}


void CdbStorageManager::schemaLoaded(SchemaManager* sc) {
	this->schemaManager = sc;

	const ArrayList<Schema, SchemaCompare>* sclist = this->schemaManager->getSchemaList();

	int maxLoop = sclist->size();
	for(int i = 0; i != maxLoop; ++i){
		Schema* schema = sclist->get(i);

		loadSchemaStore(sc, schema);
	}

}

void CdbStorageManager::loadSchemaStore(SchemaManager* scMgr, const Schema* schema) {
	const ArrayList<CdbTable, CdbTableCompare>* list = schema->getTablesList();

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		const CdbTable* table = list->get(i);

		loadTableStore(scMgr, table);
	}
}

void CdbStorageManager::loadTableStore(SchemaManager* scMgr, const CdbTable* table) {
	const File* baseDir = scMgr->getDatabaseBaseDir();

	TableStore* store = new TableStore(this->cacheManager, baseDir, table);
	StackRelease<TableStore> stStore(store);

	store->loadTable();
	stStore.cancel();

	this->tableStoreMap->put(store->getOid(), store);
}

void CdbStorageManager::onCreateTable(SchemaManager* mgr, const CdbTable* table) {
	const File* baseDir = mgr->getDatabaseBaseDir();

	TableStore* store = new TableStore(this->cacheManager, baseDir, table);
	store->createTable();
	store->loadTable();

	this->tableStoreMap->put(store->getOid(), store);
}

void CdbStorageManager::onDropTable(SchemaManager* mgr, const CdbTable* table) {
	const CdbOid* tableOid = table->getOid();

	TableStore* store = getTableStore(tableOid); __STP(store);
	this->tableStoreMap->remove(store->getOid());

	store->closeTable();

	TableStore::cleanTableStore(table, store);
}

void CdbStorageManager::onAlterModify(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table, const ColumnModifyContext* ctx) {
	const CdbOid* tableOid = table->getOid();

	TableStore* store = getTableStore(tableOid);

	handleUniqueKeyOnAlterModify(store, ctx);

	ColumnModifyContext::NotNullChage notNullChange = ctx->getNotNullChange();
	CdbTableIndex* newIndex = ctx->getNewIndex();

	bool recordModified = false;
	if(ctx->isTypeChanged() || notNullChange == ColumnModifyContext::NotNullChage::TO_NOT_NULL){
		store->resetAllIndexes();

		// convert records
		store->modifyRecords(trxManager, ctx);
		recordModified = true;
	}

	if(recordModified){
		store->buildAllIndexes();
	}
	else if(!recordModified && newIndex != nullptr){
		CdbTableIndex* newIndex = ctx->getNewIndex();
		store->buildIndex(newIndex);
	}

}

void CdbStorageManager::handleUniqueKeyOnAlterModify(TableStore* store,	const ColumnModifyContext* ctx) {
	CdbTableIndex* index = ctx->getNewIndex();
	if(index != nullptr){
		store->addNewIndex(index);
		return;
	}

	index = ctx->getRemovalIndex();
	if(index != nullptr){
		store->removeIndex(index);
	}
}

void CdbStorageManager::onDropPrimaryKey(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table, const CdbTableIndex* primaryKey) {
	const CdbOid* tableOid = table->getOid();

	TableStore* store = getTableStore(tableOid);
	store->removeIndex(primaryKey);
}

void CdbStorageManager::onAddPrimaryKey(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table, const CdbTableIndex* primaryKey) {
	const CdbOid* tableOid = table->getOid();

	TableStore* store = getTableStore(tableOid);
	store->addNewIndex(primaryKey);
	store->buildIndex(primaryKey);
}

void CdbStorageManager::onAddColumn(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table,
		const CdbTableColumn* newColumn, const CdbTableIndex* newUniqueIndex) {
	const CdbOid* tableOid = table->getOid();

	TableStore* store = getTableStore(tableOid);
	assert(store != nullptr);

	// add column
	store->addNewColumn(newColumn);

	if(newUniqueIndex != nullptr){
		store->addNewIndex(newUniqueIndex);
		store->buildIndex(newUniqueIndex);
	}
}

void CdbStorageManager::onDropColumn(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table,
		const CdbTableColumn* removalColumn, const ArrayList<CdbTableIndex>* removalIndexes) {
	const CdbOid* tableOid = table->getOid();

	TableStore* store = getTableStore(tableOid);
	assert(store != nullptr);

	store->removeColumn(removalColumn);

	int maxLoop = removalIndexes->size();
	for(int i = 0; i != maxLoop; ++i){
		const CdbTableIndex* index = removalIndexes->get(i);

		store->removeIndex(index);
	}
}

void CdbStorageManager::onAddIndex(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table,
		const CdbTableIndex* newIndex) {
	const CdbOid* tableOid = table->getOid();

	TableStore* store = getTableStore(tableOid);
	assert(store != nullptr);

	store->addNewIndex(newIndex);
	store->buildIndex(newIndex);
}

void CdbStorageManager::onDropIndex(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table,
		const CdbTableIndex* removalIndex) {
	const CdbOid* tableOid = table->getOid();

	TableStore* store = getTableStore(tableOid);
	assert(store != nullptr);

	store->removeIndex(removalIndex);
}


void CdbStorageManager::onAlterTableRenameTable(CdbTransactionManager* trxManager, SchemaManager* mgr,	const CdbTable* table, TableRenameContext* ctx) {
	const CdbOid* tableOid = table->getOid();

	TableStore* store = getTableStore(tableOid);
	store->onRename(mgr, ctx);
}

TableStore* CdbStorageManager::getTableStore(const CdbOid* tableoid) noexcept {
	return this->tableStoreMap->get(tableoid);
}

void CdbStorageManager::__testCheckIndexes() {
	Iterator<CdbOid> *it = this->tableStoreMap->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const CdbOid* oid = it->next();
		TableStore* tableStore = this->tableStoreMap->get(oid);

		tableStore->__testCheckIndexes();
	}
}

} /* namespace codablecash */
