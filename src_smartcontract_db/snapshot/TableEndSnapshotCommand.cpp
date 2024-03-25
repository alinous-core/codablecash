/*
 * TableEndSnapshotCommand.cpp
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#include "snapshot/TableEndSnapshotCommand.h"

#include "base_io/ByteBuffer.h"

#include "schema_table/schema/DatabaseEqualsChecker.h"
namespace codablecash {

TableEndSnapshotCommand::TableEndSnapshotCommand() : AbstractSnapshotCommand(AbstractSnapshotCommand::TABLE_END) {

}

TableEndSnapshotCommand::~TableEndSnapshotCommand() {

}

int TableEndSnapshotCommand::binarySize() const {
	int total = sizeof(uint8_t);

	return total;
}

void TableEndSnapshotCommand::toBinary(ByteBuffer *out) const {
	out->put(this->type);
}

void TableEndSnapshotCommand::fromBinary(ByteBuffer *in) {
}

void TableEndSnapshotCommand::recover(ISnapshotRecoverer *recoverer, CodableDatabase *db) {

}

void TableEndSnapshotCommand::__testCheckEquals(const AbstractSnapshotCommand *other) const {
	DatabaseEqualsChecker::checkIntEquals(this->type, other->getType());
}

} /* namespace codablecash */
