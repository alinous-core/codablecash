/*
 * TableStartSnapshotCommand.cpp
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#include "snapshot/TableStartSnapshotCommand.h"
#include "snapshot/ISnapshotRecoverer.h"
#include "snapshot/SchemaStartSnapshotCommand.h"

#include "base/UnicodeString.h"

#include "engine/CdbOid.h"
#include "engine/CodableDatabase.h"

#include "schema_table/table/CdbTableColumn.h"
#include "schema_table/table/TableObjectFactory.h"

#include "schema_table/schema/SchemaManager.h"

#include "schema_table/table/CdbTable.h"

#include "base/StackRelease.h"

#include "schema_table/schema/DatabaseEqualsChecker.h"
namespace codablecash {

TableStartSnapshotCommand::TableStartSnapshotCommand() : AbstractSnapshotCommand(AbstractSnapshotCommand::TABLE_START) {
	this->name = nullptr;
	this->oid = nullptr;

	this->columns = new ArrayList<CdbTableColumn>();

	this->table = nullptr;
}

TableStartSnapshotCommand::~TableStartSnapshotCommand() {
	delete this->name;
	delete this->oid;

	this->columns->deleteElements();
	delete this->columns;

	this->table = nullptr;
}

void TableStartSnapshotCommand::setOid(const CdbOid *oid) noexcept {
	delete this->oid;
	this->oid = oid->copy();
}

void TableStartSnapshotCommand::setName(const UnicodeString *name) noexcept {
	delete this->name;
	this->name = new UnicodeString(*name);
}

void TableStartSnapshotCommand::addColumn(const CdbTableColumn *col) noexcept {
	this->columns->addElement(new CdbTableColumn(*col));
}

int TableStartSnapshotCommand::binarySize() const {
	int total = sizeof(uint8_t);

	total += this->oid->binarySize();
	total += stringSize(this->name);

	total += sizeof(uint16_t);
	int maxLoop = this->columns->size();
	for(int i = 0; i != maxLoop; ++i){
		CdbTableColumn* col = this->columns->get(i);

		total += col->binarySize();
	}

	return total;
}

void TableStartSnapshotCommand::toBinary(ByteBuffer *out) const {
	out->put(this->type);

	this->oid->toBinary(out);
	putString(out, this->name);

	int maxLoop = this->columns->size();
	out->putShort(maxLoop);
	for(int i = 0; i != maxLoop; ++i){
		CdbTableColumn* col = this->columns->get(i);

		col->toBinary(out);
	}
}

void TableStartSnapshotCommand::fromBinary(ByteBuffer *in) {
	this->oid = CdbOid::fromBinary(in);
	this->name = getString(in);

	int maxLoop = in->getShort();
	for(int i = 0; i != maxLoop; ++i){
		CdbBinaryObject* obj = TableObjectFactory::createFromBinary(in, CdbTableColumn::CDB_OBJ_TYPE);
		CdbTableColumn* col = dynamic_cast<CdbTableColumn*>(obj);

		col->fromBinary(in);

		this->columns->addElement(col);
	}
}

void TableStartSnapshotCommand::recover(ISnapshotRecoverer *recoverer, CodableDatabase *db) {
	SchemaStartSnapshotCommand* schemaCmd = recoverer->getSchemaStartSnapshotCommand();
	const UnicodeString* schemaName = schemaCmd->getName();

	SchemaManager* scManager = db->getSchemaManager();
	Schema* schema = scManager->getSchema(schemaName);

	CdbTable* table = new CdbTable(this->oid->getOidValue()); __STP(table);
	table->setName(new UnicodeString(*this->name));

	int maxLoop = this->columns->size();
	for(int i = 0; i != maxLoop; ++i){
		CdbTableColumn* col = this->columns->get(i);

		table->addColumn(new CdbTableColumn(*col));
	}

	scManager->recoverTable(table);

	this->table = scManager->getTable(schemaName, this->name);
}

void TableStartSnapshotCommand::__testCheckEquals(const AbstractSnapshotCommand *other) const {
	DatabaseEqualsChecker::checkIntEquals(this->type, other->getType());

	const TableStartSnapshotCommand* cmd = dynamic_cast<const TableStartSnapshotCommand*>(other);
	DatabaseEqualsChecker::checkCdbOidEquals(this->oid, cmd->oid);
	DatabaseEqualsChecker::checkUnicodeStringEquals(this->name, cmd->name);

	DatabaseEqualsChecker::checkIntEquals(this->columns->size(), cmd->columns->size());

	int maxLoop = this->columns->size();
	for(int i = 0; i != maxLoop; ++i){
		CdbTableColumn* col = this->columns->get(i);
		CdbTableColumn* colB = cmd->columns->get(i);

		col->__testCheckEquals(colB);
	}
}

} /* namespace codablecash */
