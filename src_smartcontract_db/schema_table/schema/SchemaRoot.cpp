/*
 * SchemaBinary.cpp
 *
 *  Created on: 2020/05/11
 *      Author: iizuka
 */

#include "schema_table/schema/SchemaRoot.h"
#include "schema_table/schema/Schema.h"
#include "schema_table/schema/SchemaCompare.h"

#include "base/UnicodeString.h"

#include "base_io/ByteBuffer.h"

#include "schema_table/table/TableObjectFactory.h"
#include "schema_table/table/CdbTable.h"

#include "engine/CdbException.h"

#include "snapshot/ISnapshotHandler.h"

#include "snapshot/IdsBackupCommand.h"

#include "engine/CdbOid.h"

#include "schema_table/schema/DatabaseEqualsChecker.h"

#include "base/StackRelease.h"
namespace codablecash {

SchemaRoot::SchemaRoot() {
	this->maxTransactionId = 0;
	this->maxObjectId = 0;
	this->maxSchemaObjectId = 0;
	this->maxCommitId = 0;
}

SchemaRoot::~SchemaRoot() {
	this->list.deleteElements();
}

void SchemaRoot::addSchemaName(const UnicodeString* name) noexcept {
	uint64_t newOid = newSchemaObjectId();

	Schema* schema = new Schema(newOid);
	schema->setName(new UnicodeString(name));

	addSchema(schema);
}

Schema* SchemaRoot::getSchema(const UnicodeString* name) const noexcept{
	return this->map.get(name);
}

void SchemaRoot::addSchema(Schema* schema) noexcept {
	this->list.addElement(schema);
	this->map.put(schema->getName(), schema);

	this->list.sort();
}

const ArrayList<Schema, SchemaCompare>* SchemaRoot::getSchemaList() const noexcept {
	return &this->list;
}

void SchemaRoot::removeSchema(const UnicodeString *name) noexcept {
	Schema* schema = this->map.get(name); __STP(schema);
	this->map.remove(name);

	this->list.removeByObj(schema);
}

int SchemaRoot::binarySize() const {
	int total = 0;

	total += sizeof(uint64_t) * 4;

	total += sizeof(uint32_t);
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		Schema* schema = this->list.get(i);

		total += schema->binarySize();
	}

	return total;
}

void SchemaRoot::toBinary(ByteBuffer* out) const {
	out->putLong(this->maxTransactionId);
	out->putLong(this->maxObjectId);
	out->putLong(this->maxSchemaObjectId);
	out->putLong(this->maxCommitId);

	int maxLoop = this->list.size();
	out->putInt(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		Schema* schema = this->list.get(i);

		schema->toBinary(out);
	}
}

void SchemaRoot::fromBinary(ByteBuffer* in) {
	this->maxTransactionId = in->getLong();
	this->maxObjectId = in->getLong();
	this->maxSchemaObjectId = in->getLong();
	this->maxCommitId = in->getLong();

	int maxLoop = in->getInt();
	for(int i = 0; i != maxLoop; ++i){
		CdbBinaryObject* obj = TableObjectFactory::createFromBinary(in, Schema::CDB_OBJ_TYPE);
		Schema* schema = dynamic_cast<Schema*>(obj);

		schema->fromBinary(in);

		addSchema(schema);
	}
}

uint64_t SchemaRoot::newTransactionId() noexcept {
	this->maxTransactionId++;
	return this->maxTransactionId;
}

uint64_t SchemaRoot::newSchemaObjectId() noexcept {
	this->maxSchemaObjectId++;
	return this->maxSchemaObjectId;
}

uint64_t SchemaRoot::newRecordObjectId() noexcept {
	this->maxObjectId++;
	return this->maxObjectId;
}

uint64_t SchemaRoot::newCommitId() noexcept {
	this->maxCommitId++;
	return this->maxCommitId;
}

uint64_t SchemaRoot::getSchemaObjectVersionId() const noexcept {
	return this->maxSchemaObjectId;
}

void SchemaRoot::upgradeSchemaObjectVersionId() noexcept {
	this->maxSchemaObjectId++;
}

const CdbTable* SchemaRoot::createTable(const CdbTable* table) {
	const UnicodeString* schemaName = table->getSchemaName();
	Schema* sc = getSchema(schemaName);
	if(sc == nullptr){
		throw new CdbException(L"Schema does not exists.", __FILE__, __LINE__);
	}

	CdbTable* newTable = new CdbTable(*table);

	sc->addTable(newTable);

	return newTable;
}

const CdbTable* SchemaRoot::recoverTable(const CdbTable *table) {
	const UnicodeString* schemaName = table->getSchemaName();
	Schema* sc = getSchema(schemaName);

	CdbTable* newTable = new CdbTable(*table);
	sc->addTable(newTable);

	return newTable;
}

void SchemaRoot::removeTable(const UnicodeString* schema, const UnicodeString* table) {
	Schema* sc = getSchema(schema);
	assert(sc != nullptr);

	sc->removeTable(table);
}

CdbTable* SchemaRoot::getTable(const CdbOid *tableOid) const {
	CdbTable* ret = nullptr;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		Schema* sc = this->list.get(i);

		ret = getTable(sc, tableOid);
		if(ret != nullptr){
			break;
		}
	}

	return ret;
}

CdbTable* SchemaRoot::getTable(Schema *sc, const CdbOid *tableOid) const noexcept {
	CdbTable* ret = nullptr;

	const ArrayList<CdbTable, CdbTableCompare>* tables = sc->getTablesList();
	int maxLoop = tables->size();
	for(int i = 0; i != maxLoop; ++i){
		CdbTable* table = tables->get(i);

		if(tableOid->equals(table->getOid())){
			ret = table;
			break;
		}
	}

	return ret;
}

void SchemaRoot::snapshotObjectIds(ISnapshotHandler *handler) noexcept {
	IdsBackupCommand cmd;
	cmd.setValues(this->maxTransactionId, this->maxObjectId, this->maxSchemaObjectId, this->maxCommitId);

	handler->putCommand(&cmd);
}

void SchemaRoot::__testCheckEqualsObjectIds(SchemaRoot *other) {
	DatabaseEqualsChecker::checkLongEquals(this->maxTransactionId, other->maxTransactionId);
	DatabaseEqualsChecker::checkLongEquals(this->maxObjectId, other->maxObjectId);
	DatabaseEqualsChecker::checkLongEquals(this->maxSchemaObjectId, other->maxSchemaObjectId);
	DatabaseEqualsChecker::checkLongEquals(this->maxCommitId, other->maxCommitId);
}

void SchemaRoot::snapshot(ISnapshotHandler *handler, CdbStorageManager *storeManager) {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		Schema* schema = this->list.get(i);

		schema->snapshot(handler, storeManager);
	}
}

void SchemaRoot::__testCheckEquals(CdbStorageManager *storeManager, SchemaRoot *other, CdbStorageManager *otherStoreManager) {
	DatabaseEqualsChecker::checkIntEquals(this->list.size(), other->list.size());

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		Schema* schema = this->list.get(i);
		Schema* otherSchema = other->list.get(i);

		schema->__testCheckEquals(storeManager, otherSchema, otherStoreManager);
	}
}

void SchemaRoot::recoverObjectIds(uint64_t maxTransactionId, uint64_t maxObjectId, uint64_t maxSchemaObjectId, uint64_t maxCommitId) noexcept {
	this->maxTransactionId = maxTransactionId;
	this->maxObjectId = maxObjectId;
	this->maxSchemaObjectId = maxSchemaObjectId;
	this->maxCommitId = maxCommitId;
}

} /* namespace codablecash */
