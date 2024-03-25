/*
 * Schema.cpp
 *
 *  Created on: 2020/05/16
 *      Author: iizuka
 */

#include "schema_table/schema/Schema.h"

#include "base/UnicodeString.h"
#include "base/Iterator.h"
#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"

#include "engine/CdbOid.h"
#include "engine/OidWraper.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/TableObjectFactory.h"

#include "snapshot/ISnapshotHandler.h"
#include "snapshot/SchemaStartSnapshotCommand.h"

#include "snapshot/SchemaEndSnapshotCommand.h"

#include "schema_table/schema/DatabaseEqualsChecker.h"


namespace codablecash {

Schema::Schema(uint64_t oid) {
	this->name = nullptr;
	this->oid = new CdbOid(oid);
	this->tables = new ArrayList<CdbTable, CdbTableCompare>();
}

Schema::~Schema() {
	delete this->name;
	delete this->oid;

	this->tables->deleteElements();
	delete this->tables;
}
/*
void Schema::setOid(uint64_t oid) noexcept {
	delete this->oid;
	this->oid = new CdbOid(oid);
}
*/
void Schema::setName(UnicodeString* name) noexcept {
	this->name = name;
}

void Schema::addTable(CdbTable* table) noexcept {
	table->setSchema(this);

	this->tables->addElement(table);
	this->nameTableMap.put(table->getName(), table);

	OidWraper wrapper(table->getOid());
	this->oidTableMap.put(&wrapper, table);

	this->tables->sort();
}

const ArrayList<CdbTable, CdbTableCompare>* Schema::getTablesList() const noexcept {
	return this->tables;
}

int Schema::binarySize() const {
	checkNotNull(this->name);

	int total = sizeof(uint8_t);
	total += sizeof(uint64_t); // oid

	total += stringSize(this->name);

	total += sizeof(int32_t); // numTable

	Iterator<OidWraper>* it = this->oidTableMap.keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const OidWraper* wrapper = it->next();
		CdbTable* table = this->oidTableMap.get(wrapper);

		total += table->binarySize();
	}

	return total;
}

void Schema::toBinary(ByteBuffer* out) const {
	checkNotNull(this->name);

	out->put(Schema::CDB_OBJ_TYPE);
	out->putLong(this->oid->getOidValue());

	putString(out, this->name);

	int numTable = this->oidTableMap.size();
	out->putInt(numTable);

	Iterator<OidWraper>* it = this->oidTableMap.keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const OidWraper* oid = it->next();
		CdbTable* table = this->oidTableMap.get(oid);

		table->toBinary(out);
	}
}

void Schema::fromBinary(ByteBuffer* in) {
	this->name = getString(in);

	int numTable = in->getInt();
	for(int i = 0; i != numTable; ++i){
		CdbBinaryObject* obj = TableObjectFactory::createFromBinary(in, CdbTable::CDB_OBJ_TYPE);
		CdbTable* table = dynamic_cast<CdbTable*>(obj);

		table->fromBinary(in);

		addTable(table);
	}
}


CdbTable* Schema::getCdbTableByName(const UnicodeString* tableName) const noexcept {
	return this->nameTableMap.get(tableName);
}

void Schema::removeTable(const UnicodeString* tableName) noexcept {
	CdbTable* table = this->nameTableMap.get(tableName);

	this->nameTableMap.remove(tableName);

	OidWraper wrapper(table->getOid());
	this->oidTableMap.remove(&wrapper);

	tables->removeByObj(table);
}


void Schema::renameTable(const UnicodeString* lastName,	const UnicodeString* newName) noexcept {
	CdbTable* table = this->nameTableMap.get(lastName);
	this->nameTableMap.remove(lastName);

	table->setName(new UnicodeString(newName));
	this->nameTableMap.put(newName, table);
}

void Schema::snapshot(ISnapshotHandler *handler,	CdbStorageManager *storeManager) {
	{
		SchemaStartSnapshotCommand cmd;
		cmd.setOid(this->oid);
		cmd.setName(this->name);
		handler->putCommand(&cmd);
	}


	int maxLoop = this->tables->size();
	for(int i = 0; i != maxLoop; ++i){
		CdbTable* table = this->tables->get(i);

		table->snapshot(handler, storeManager);
	}

	{
		SchemaEndSnapshotCommand cmd;
		handler->putCommand(&cmd);
	}
}

void Schema::__testCheckEquals(CdbStorageManager *storeManager, Schema *other, CdbStorageManager *otherStoreManager) {
	DatabaseEqualsChecker::checkIntEquals(this->tables->size(), other->tables->size());

	int maxLoop = this->tables->size();
	for(int i = 0; i != maxLoop; ++i){
		CdbTable* table = this->tables->get(i);
		CdbTable* otherTable = other->tables->get(i);

		table->__testCheckEquals(storeManager, otherTable, otherStoreManager);
	}

}

} /* namespace codablecash */
