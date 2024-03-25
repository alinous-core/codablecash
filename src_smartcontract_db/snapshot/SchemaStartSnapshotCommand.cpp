/*
 * SchemaEndSnapshotCommand.cpp
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#include "snapshot/SchemaStartSnapshotCommand.h"

#include "engine/CdbOid.h"
#include "engine/CodableDatabase.h"

#include "base/UnicodeString.h"

#include "schema_table/schema/Schema.h"
#include "schema_table/schema/SchemaManager.h"

#include "schema_table/schema/DatabaseEqualsChecker.h"
namespace codablecash {

SchemaStartSnapshotCommand::SchemaStartSnapshotCommand() : AbstractSnapshotCommand(AbstractSnapshotCommand::SCHEMA_START) {
	this->name = nullptr;
	this->oid = nullptr;
}

SchemaStartSnapshotCommand::~SchemaStartSnapshotCommand() {
	delete this->name;
	delete this->oid;
}

void SchemaStartSnapshotCommand::setOid(const CdbOid *oid) noexcept {
	delete this->oid;
	this->oid = oid->copy();
}

void SchemaStartSnapshotCommand::setName(const UnicodeString *name) noexcept {
	delete this->name;
	this->name = new UnicodeString(*name);
}

int SchemaStartSnapshotCommand::binarySize() const {
	int total = sizeof(uint8_t);

	total += this->oid->binarySize();
	total += stringSize(this->name);

	return total;
}

void SchemaStartSnapshotCommand::toBinary(ByteBuffer *out) const {
	out->put(this->type);

	this->oid->toBinary(out);
	putString(out, this->name);
}

void SchemaStartSnapshotCommand::fromBinary(ByteBuffer *in) {
	this->oid = CdbOid::fromBinary(in);
	this->name = getString(in);
}

void SchemaStartSnapshotCommand::recover(ISnapshotRecoverer *recoverer, CodableDatabase *db) {
	SchemaManager* scManager = db->getSchemaManager();

	Schema* schema = new Schema(this->oid->getOidValue());
	schema->setName(new UnicodeString(*this->name));

	scManager->recoverSchema(schema);
}

void SchemaStartSnapshotCommand::__testCheckEquals(const AbstractSnapshotCommand *other) const {
	DatabaseEqualsChecker::checkIntEquals(this->type, other->getType());

	const SchemaStartSnapshotCommand* cmd = dynamic_cast<const SchemaStartSnapshotCommand*>(other);

	DatabaseEqualsChecker::checkCdbOidEquals(this->oid, cmd->oid);
	DatabaseEqualsChecker::checkUnicodeStringEquals(this->name, cmd->name);
}

} /* namespace codablecash */
