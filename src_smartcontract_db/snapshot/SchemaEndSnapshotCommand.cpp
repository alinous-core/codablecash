/*
 * SchemaStartSnapshotCommand.cpp
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#include "snapshot/SchemaEndSnapshotCommand.h"

#include "base_io/ByteBuffer.h"

#include "schema_table/schema/DatabaseEqualsChecker.h"
namespace codablecash {

SchemaEndSnapshotCommand::SchemaEndSnapshotCommand() : AbstractSnapshotCommand(AbstractSnapshotCommand::SCHEMA_END) {

}

SchemaEndSnapshotCommand::~SchemaEndSnapshotCommand() {

}

void SchemaEndSnapshotCommand::recover(ISnapshotRecoverer* recoverer, CodableDatabase* db) {
}

int SchemaEndSnapshotCommand::binarySize() const {
	int total = sizeof(uint8_t);

	return total;
}

void SchemaEndSnapshotCommand::toBinary(ByteBuffer *out) const {
	out->put(this->type);
}

void SchemaEndSnapshotCommand::fromBinary(ByteBuffer *in) {
}

void SchemaEndSnapshotCommand::__testCheckEquals(const AbstractSnapshotCommand *other) const {
	DatabaseEqualsChecker::checkIntEquals(this->type, other->getType());
}

} /* namespace codablecash */
