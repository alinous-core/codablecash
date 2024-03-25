/*
 * Schema.cpp
 *
 *  Created on: 2020/05/11
 *      Author: iizuka
 */

#include "schema_table/schema/SchemaManager.h"
#include "schema_table/schema/SchemaRoot.h"
#include "schema_table/schema/ISchemaUptateListner.h"
#include "schema_table/schema/Schema.h"
#include "schema_table/schema/SchemaCompare.h"

#include "schema_table/schema_alter_ctx/ColumnModifyContext.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "base_io/File.h"
#include "base_io/ByteBuffer.h"

#include "base_io_stream/FileOutputStream.h"
#include "base_io_stream/FileInputStream.h"

#include "engine/CdbException.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/CdbTableColumn.h"
#include "schema_table/table/CdbTableIndex.h"

#include "lang_sql/sql_ddl_alter_modify/AlterModifyCommand.h"

#include "trx/transaction_log_alter_modify/AlterModifyCommandLog.h"

#include "lang_sql/sql_join_parts/TableIdentifier.h"

#include "trx/transaction_log_alter/AlterAddColumnCommandLog.h"
#include "trx/transaction_log_alter/AlterAddIndexCommandLog.h"
#include "trx/transaction_log_alter/AlterDropIndexCommandLog.h"
#include "trx/transaction_log_alter/AlterDropColumnCommandLog.h"
#include "trx/transaction_log_alter_modify/AlterAddPrimaryKeyCommandLog.h"
#include "trx/transaction_log_alter_modify/AlterDropPrimaryKeyCommandLog.h"
#include "trx/transaction_log_alter_modify/AlterRenameColumnCommandLog.h"
#include "trx/transaction_log_alter_modify/AlterRenameTableCommandLog.h"

#include "lang_sql/sql_ddl/DdlColumnDescriptor.h"
#include "lang_sql/sql_ddl/ColumnTypeDescriptor.h"

#include "schema_table/schema/SchemaAlterCommandsHandler.h"

namespace codablecash {

const UnicodeString SchemaManager::PUBLIC(L"public");
const UnicodeString SchemaManager::SCHEMA_FILE(L"schema.bin");


SchemaManager::SchemaManager(CodableDatabase* db) {
	this->root = nullptr;
	this->schemaBin = nullptr;
	this->databaseBaseDir = nullptr;
	this->alterHandler = new SchemaAlterCommandsHandler(this);
}

SchemaManager::~SchemaManager() {
	delete this->root;
	this->root = nullptr;

	delete this->schemaBin;
	this->schemaBin = nullptr;

	delete this->databaseBaseDir;
	this->databaseBaseDir = nullptr;

	delete this->alterHandler;
	this->alterHandler = nullptr;
}

void SchemaManager::addSchemaUpdateListner(ISchemaUptateListner* listner) noexcept {
	this->listners.addElement(listner);
}

void SchemaManager::createBlankSchema(File *baseDir) {
	SchemaRoot root;

	File* schemaBin = baseDir->get(&SchemaManager::SCHEMA_FILE); __STP(schemaBin);

	FileOutputStream* outStream = new FileOutputStream(schemaBin); __STP(outStream);
	outStream->open(false);

	int size = root.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	root.toBinary(buff);

	const uint8_t* barray = buff->array();
	outStream->write((const char*)barray, size);
	outStream->flush();

	outStream->close();
}

void SchemaManager::createSchema(const UnicodeString* name, File* baseDir) {
	SchemaRoot root;

	root.addSchemaName(&SchemaManager::PUBLIC);
	File* scdir = baseDir->get(&SchemaManager::PUBLIC); __STP(scdir);

	scdir->mkdirs();

	File* schemaBin = baseDir->get(&SchemaManager::SCHEMA_FILE); __STP(schemaBin);

	FileOutputStream* outStream = new FileOutputStream(schemaBin); __STP(outStream);
	outStream->open(false);

	int size = root.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	root.toBinary(buff);

	const uint8_t* barray = buff->array();
	outStream->write((const char*)barray, size);
	outStream->flush();

	outStream->close();
}


void SchemaManager::createSchema(const UnicodeString* name) {
	Schema* schema = new Schema(newSchemaObjectId());
	schema->setName(new UnicodeString(name));

	File* scdir = this->databaseBaseDir->get(name); __STP(scdir);
	scdir->mkdirs();

	this->root->addSchema(schema);
}

void SchemaManager::recoverSchema(Schema *schema) {
	const UnicodeString* name = schema->getName();

	File* scdir = this->databaseBaseDir->get(name); __STP(scdir);
	scdir->mkdirs();

	this->root->addSchema(schema);
}

void SchemaManager::save() {
	FileOutputStream* outStream = new FileOutputStream(schemaBin); __STP(outStream);
	outStream->open(false);

	int size = this->root->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	this->root->toBinary(buff);

	const uint8_t* barray = buff->array();
	outStream->write((const char*)barray, size);
	//outStream->flush();

	outStream->close();
}


void SchemaManager::loadSchema(const File* baseDir) {
	this->databaseBaseDir = new File(*baseDir);
	this->schemaBin = baseDir->get(&SchemaManager::SCHEMA_FILE);

	int size = schemaBin->length();
	char* b = new char[size]{};
	StackArrayRelease<char> br(b);

	FileInputStream* inStream = new FileInputStream(schemaBin); __STP(inStream);
	inStream->open();

	inStream->read(b, size);

	inStream->close();

	ByteBuffer* buff = ByteBuffer::wrapWithEndian((const uint8_t*)b, size, true); __STP(buff);

	this->root = new SchemaRoot();
	this->root->fromBinary(buff);

	fireSchemaLoaded();
}

uint64_t SchemaManager::newTransactionId() {
	uint64_t trxId = this->root->newTransactionId();
	save();

	return trxId;
}

uint64_t SchemaManager::newSchemaObjectId() noexcept {
	return this->root->newSchemaObjectId();
}

uint64_t SchemaManager::newRecordObjectId() noexcept {
	return this->root->newRecordObjectId();
}

uint64_t SchemaManager::newCommitId() noexcept {
	return this->root->newCommitId();
}

uint64_t SchemaManager::getSchemaObjectVersionId() const noexcept {
	return this->root->getSchemaObjectVersionId();
}

void SchemaManager::snapshotObjectIds(ISnapshotHandler *handler) noexcept {
	this->root->snapshotObjectIds(handler);
}

void SchemaManager::__testCheckEqualsObjectIds(SchemaManager *other) {
	this->root->__testCheckEqualsObjectIds(other->root);
}

void SchemaManager::__testCheckEquals(CdbStorageManager* storeManager, SchemaManager* other, CdbStorageManager* otherStoreManager) {
	this->root->__testCheckEquals(storeManager, other->root, otherStoreManager);
}

void SchemaManager::createTable(CdbTable* table) {
	const CdbTable* newTable = this->root->createTable(table);
	fireOnCreateTable(newTable);

	save();
}

void SchemaManager::recoverTable(CdbTable *table) {
	const CdbTable* newTable = this->root->recoverTable(table);
	fireOnCreateTable(newTable);

	save();
}

void SchemaManager::recoverIndex(CdbTransactionManager* trxManager, CdbTableIndex *index, const UnicodeString *schemaName, const UnicodeString *tableName) {
	CdbTable* table = getTable(schemaName, tableName);

	table->addIndex(index);

	fireOnAddIndex(trxManager, table, index);

	// save
	save();
}

void SchemaManager::dropTable(const TableIdentifier* tableId){
	const UnicodeString* schema = tableId->getSchema();
	const UnicodeString* tableName = tableId->getTableName();

	CdbTable* table = getTable(schema, tableName); __STP(table);

	this->root->removeTable(schema, tableName);

	fireOnDropTable(table);

	// upgrade & save
	this->root->upgradeSchemaObjectVersionId();
	save();
}

void SchemaManager::removeTable(const UnicodeString *schema, const UnicodeString *name) {
	CdbTable* table = getTable(schema, name); __STP(table);

	this->root->removeTable(schema, name);

	fireOnDropTable(table);
	save();
}

void SchemaManager::fireSchemaLoaded() noexcept {
	int maxLoop = this->listners.size();
	for(int i = 0; i != maxLoop; ++i){
		ISchemaUptateListner* l = this->listners.get(i);
		l->schemaLoaded(this);
	}
}

CdbTable* SchemaManager::findTableFromCommand(const AbstractAlterCommandLog* cmdlog) {
	const TableIdentifier* tableId = cmdlog->getTableId();

	const UnicodeString* schema = tableId->getSchema();
	const UnicodeString* tableName = tableId->getTableName();

	CdbTable* table = getTable(schema, tableName);

	return table;
}

Schema* SchemaManager::getSchema(const UnicodeString* name) const noexcept {
	return this->root->getSchema(name);
}

const ArrayList<Schema, SchemaCompare>* SchemaManager::getSchemaList() const noexcept {
	return this->root->getSchemaList();
}

void SchemaManager::removeSchema(const UnicodeString *name) noexcept {
	this->root->removeSchema(name);
}

CdbTable* SchemaManager::getTable(const wchar_t* schema, const wchar_t* name) const {
	UnicodeString s(schema);
	UnicodeString n(name);

	return getTable(&s, &n);
}

CdbTable* SchemaManager::getTable(const TableIdentifier* tableId, const UnicodeString* defaultSchema) const {
	const UnicodeString* schema = tableId->getSchema();
	const UnicodeString* table = tableId->getTableName();

	if(schema == nullptr){
		schema = defaultSchema;
	}

	return getTable(schema, table);
}

CdbTable* SchemaManager::getTable(const UnicodeString* schema, const UnicodeString* name) const {
	Schema* sc = getSchema(schema);
	if(sc == nullptr){
		throw new CdbException(L"schema does not exists.", __FILE__, __LINE__);
	}

	CdbTable* table = sc->getCdbTableByName(name);
	if(table == nullptr){
		throw new CdbException(L"schema does not exists.", __FILE__, __LINE__);
	}

	return table;
}

CdbTable* SchemaManager::getTable(const CdbOid *tableOid) const {
	return this->root->getTable(tableOid);
}

bool SchemaManager::hasTable(const TableIdentifier* tableId) const noexcept {
	const UnicodeString* schema = tableId->getSchema();
	const UnicodeString* name = tableId->getTableName();

	return hasTable(schema, name);
}

bool SchemaManager::hasTable(const UnicodeString* schema, const UnicodeString* name) const noexcept {
	Schema* sc = getSchema(schema);
	if(sc == nullptr){
		return false;
	}

	CdbTable* table = sc->getCdbTableByName(name);

	return table != nullptr;
}


void SchemaManager::fireOnCreateTable(const CdbTable* table) {
	int maxLoop = this->listners.size();
	for(int i = 0; i != maxLoop; ++i){
		ISchemaUptateListner* l = this->listners.get(i);
		l->onCreateTable(this, table);
	}
}

void SchemaManager::fireOnDropTable(const CdbTable* table) {
	int maxLoop = this->listners.size();
	for(int i = 0; i != maxLoop; ++i){
		ISchemaUptateListner* l = this->listners.get(i);
		l->onDropTable(this, table);
	}
}

void SchemaManager::fireOnAlterModify(CdbTransactionManager* trxManager, const CdbTable* table, const ColumnModifyContext* ctx) {
	int maxLoop = this->listners.size();
	for(int i = 0; i != maxLoop; ++i){
		ISchemaUptateListner* l = this->listners.get(i);
		l->onAlterModify(trxManager, this, table, ctx);
	}
}

void SchemaManager::fireOnDropPrimaryKey(CdbTransactionManager* trxManager, const CdbTable* table, const CdbTableIndex* primaryKey) {
	int maxLoop = this->listners.size();
	for(int i = 0; i != maxLoop; ++i){
		ISchemaUptateListner* l = this->listners.get(i);
		l->onDropPrimaryKey(trxManager, this, table, primaryKey);
	}
}

void SchemaManager::fireOnAddPrimaryKey(CdbTransactionManager* trxManager, const CdbTable* table,	const CdbTableIndex* primaryKey) {
	int maxLoop = this->listners.size();
	for(int i = 0; i != maxLoop; ++i){
		ISchemaUptateListner* l = this->listners.get(i);
		l->onAddPrimaryKey(trxManager, this, table, primaryKey);
	}
}

void SchemaManager::fireOnAddColumn(CdbTransactionManager* trxManager, const CdbTable* table,
		const CdbTableColumn* newColumn, const CdbTableIndex* newUniqueIndex) {
	int maxLoop = this->listners.size();
	for(int i = 0; i != maxLoop; ++i){
		ISchemaUptateListner* l = this->listners.get(i);
		l->onAddColumn(trxManager, this, table, newColumn, newUniqueIndex);
	}
}

void SchemaManager::fireOnDropColumn(CdbTransactionManager* trxManager, const CdbTable* table,
		const CdbTableColumn* removalColumn, const ArrayList<CdbTableIndex>* removalIndexes) {
	int maxLoop = this->listners.size();
	for(int i = 0; i != maxLoop; ++i){
		ISchemaUptateListner* l = this->listners.get(i);
		l->onDropColumn(trxManager, this, table, removalColumn, removalIndexes);
	}
}

void SchemaManager::fireOnAddIndex(CdbTransactionManager* trxManager, const CdbTable* table,
		const CdbTableIndex* newIndex) {
	int maxLoop = this->listners.size();
	for(int i = 0; i != maxLoop; ++i){
		ISchemaUptateListner* l = this->listners.get(i);
		l->onAddIndex(trxManager, this, table, newIndex);
	}
}

void SchemaManager::fireOnDropIndex(CdbTransactionManager* trxManager, const CdbTable* table,
		const CdbTableIndex* removalIndex) {
	int maxLoop = this->listners.size();
	for(int i = 0; i != maxLoop; ++i){
		ISchemaUptateListner* l = this->listners.get(i);
		l->onDropIndex(trxManager, this, table, removalIndex);
	}
}


void SchemaManager::fireOnRenameTable(CdbTransactionManager* trxManager, const CdbTable* table, TableRenameContext* context) {
	int maxLoop = this->listners.size();
	for(int i = 0; i != maxLoop; ++i){
		ISchemaUptateListner* l = this->listners.get(i);
		l->onAlterTableRenameTable(trxManager, this, table, context);
	}
}


void SchemaManager::handleAlterTableAddIndex(const AlterAddIndexCommandLog* cmd, CdbTransactionManager* trxManager) {
	this->alterHandler->handleAlterTableAddIndex(cmd, trxManager);
}

void SchemaManager::handleAlterTableAddColumn(const AlterAddColumnCommandLog* cmd, CdbTransactionManager* trxManager) {
	this->alterHandler->handleAlterTableAddColumn(cmd, trxManager);
}

void SchemaManager::handleAlterTableDropIndex(const AlterDropIndexCommandLog* cmd, CdbTransactionManager* trxManager) {
	this->alterHandler->handleAlterTableDropIndex(cmd, trxManager);
}

void SchemaManager::handleAlterTableDropColumn(const AlterDropColumnCommandLog* cmd, CdbTransactionManager* trxManager) {
	this->alterHandler->handleAlterTableDropColumn(cmd, trxManager);
}

void SchemaManager::handleAlterTableAddPrimaryKey(const AlterAddPrimaryKeyCommandLog* cmd, CdbTransactionManager* trxManager) {
	this->alterHandler->handleAlterTableAddPrimaryKey(cmd, trxManager);
}

void SchemaManager::handleAlterTableDropPrimaryKey(const AlterDropPrimaryKeyCommandLog* cmd, CdbTransactionManager* trxManager) {
	this->alterHandler->handleAlterTableDropPrimaryKey(cmd, trxManager);
}

void SchemaManager::handleAlterTableModify(const AlterModifyCommandLog* cmd, CdbTransactionManager* trxManager) {
	this->alterHandler->handleAlterTableModify(cmd, trxManager);
}

void SchemaManager::handleAlterTableRenameColumn(const AlterRenameColumnCommandLog* cmd, CdbTransactionManager* trxManager) {
	this->alterHandler->handleAlterTableRenameColumn(cmd, trxManager);
}

void SchemaManager::handleAlterTableRenameTable(const AlterRenameTableCommandLog* cmd, CdbTransactionManager* trxManager) {
	this->alterHandler->handleAlterTableRenameTable(cmd, trxManager);
}

void SchemaManager::snapshot(ISnapshotHandler *handler,	CdbStorageManager *storeManager) {
	this->root->snapshot(handler, storeManager);
}

void SchemaManager::recoverObjectIds(uint64_t maxTransactionId, uint64_t maxObjectId, uint64_t maxSchemaObjectId, uint64_t maxCommitId) {
	this->root->recoverObjectIds(maxTransactionId, maxObjectId, maxSchemaObjectId, maxCommitId);

	save();
}

} /* namespace alinous */
