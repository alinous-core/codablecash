/*
 * SnapshotIndexCommand.cpp
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#include "snapshot/SnapshotIndexCommand.h"
#include "snapshot/SchemaStartSnapshotCommand.h"
#include "snapshot/ISnapshotRecoverer.h"
#include "snapshot/TableStartSnapshotCommand.h"

#include "schema_table/table/CdbTableColumn.h"
#include "schema_table/table/TableObjectFactory.h"
#include "schema_table/table/CdbTableIndex.h"

#include "schema_table/schema/SchemaManager.h"

#include "base/UnicodeString.h"

#include "engine/CdbOid.h"
#include "engine/CodableDatabase.h"

#include "schema_table/schema/DatabaseEqualsChecker.h"

#include "bc_base/BinaryUtils.h"
namespace codablecash {

SnapshotIndexCommand::SnapshotIndexCommand() : AbstractSnapshotCommand(AbstractSnapshotCommand::INDEX) {
	this->oid = nullptr;
	this->name = nullptr;
	this->primary = false;
	this->unique = false;

	this->columns = new ArrayList<CdbOid>();
}

SnapshotIndexCommand::~SnapshotIndexCommand() {
	delete this->oid;
	delete this->name;

	this->columns->deleteElements();
	delete this->columns;
}

void SnapshotIndexCommand::setOid(const CdbOid *oid) noexcept {
	delete this->oid;
	this->oid = oid->copy();
}

void SnapshotIndexCommand::setName(const UnicodeString *name) noexcept {
	delete this->name;
	this->name = new UnicodeString(*name);
}

void SnapshotIndexCommand::setPrimary(bool b) noexcept {
	this->primary = b;
}

void SnapshotIndexCommand::setUnique(bool b) noexcept {
	this->unique = b;
}

void SnapshotIndexCommand::addColumn(const CdbTableColumn *col) noexcept {
	this->columns->addElement(col->getOid()->copy());
}

int SnapshotIndexCommand::binarySize() const {
	BinaryUtils::checkNotNull(this->oid);
	BinaryUtils::checkNotNull(this->name);

	int total = sizeof(uint8_t);

	total += this->oid->binarySize();
	total += stringSize(this->name);

	total += sizeof(uint8_t);
	total += sizeof(uint8_t);

	int maxLoop = this->columns->size();
	total += sizeof(uint16_t);
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* col = this->columns->get(i);

		total += col->binarySize();
	}

	return total;
}

void SnapshotIndexCommand::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->oid);
	BinaryUtils::checkNotNull(this->name);

	out->put(this->type);

	this->oid->toBinary(out);
	putString(out, this->name);

	out->put(this->primary ? 1 : 0);
	out->put(this->unique ? 1 : 0);

	int maxLoop = this->columns->size();
	out->putShort(maxLoop);
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* col = this->columns->get(i);

		col->toBinary(out);
	}
}

void SnapshotIndexCommand::fromBinary(ByteBuffer *in) {
	this->oid = CdbOid::fromBinary(in);
	this->name = getString(in);

	uint8_t bl = in->get();
	this->primary = bl > 0;

	bl = in->get();
	this->unique = bl > 0;

	int maxLoop = in->getShort();
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* col = CdbOid::fromBinary(in);
		BinaryUtils::checkNotNull(col);

		this->columns->addElement(col);
	}
}

void SnapshotIndexCommand::recover(ISnapshotRecoverer *recoverer,	CodableDatabase *db) {
	SchemaStartSnapshotCommand* schemaCmd = recoverer->getSchemaStartSnapshotCommand();
	const UnicodeString* schemaName = schemaCmd->getName();

	TableStartSnapshotCommand* tableCmd = recoverer->getTableStartSnapshotCommand();
	const UnicodeString* tableName = tableCmd->getName();

	CdbTableIndex* newIndex = new CdbTableIndex(this->oid->getOidValue());
	newIndex->setName(new UnicodeString(*this->name));
	newIndex->setPrimaryKey(this->primary);
	newIndex->setUnique(this->unique);

	int maxLoop = this->columns->size();
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* col = this->columns->get(i);

		newIndex->addColumn(col);
	}

	SchemaManager* scManager = db->getSchemaManager();
	CdbTransactionManager* trxManager = db->getTransactionxManager();
	scManager->recoverIndex(trxManager, newIndex, schemaName, tableName);
}

void SnapshotIndexCommand::__testCheckEquals(	const AbstractSnapshotCommand *other) const {
	DatabaseEqualsChecker::checkIntEquals(this->type, other->getType());

	const SnapshotIndexCommand* cmd = dynamic_cast<const SnapshotIndexCommand*>(other);

	DatabaseEqualsChecker::checkCdbOidEquals(this->oid, cmd->oid);
	DatabaseEqualsChecker::checkUnicodeStringEquals(this->name, cmd->name);
	DatabaseEqualsChecker::checkBoolEquals(this->primary, cmd->primary);
	DatabaseEqualsChecker::checkBoolEquals(this->unique, cmd->unique);

	DatabaseEqualsChecker::checkIntEquals(this->columns->size(), cmd->columns->size());

	int maxLoop = this->columns->size();
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* col = this->columns->get(i);
		CdbOid* colB = cmd->columns->get(i);

		DatabaseEqualsChecker::checkCdbOidEquals(col, colB);
	}
}

} /* namespace codablecash */
