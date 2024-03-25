/*
 * IdsBackupCommand.cpp
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#include "snapshot/IdsBackupCommand.h"

#include "base_io/ByteBuffer.h"

#include "engine/CodableDatabase.h"

#include "schema_table/schema/SchemaManager.h"

#include "schema_table/schema/DatabaseEqualsChecker.h"

#include "snapshot/AbstractSnapshotCommand.h"
namespace codablecash {

IdsBackupCommand::IdsBackupCommand() : AbstractSnapshotCommand(AbstractSnapshotCommand::IDS_BACKUP) {
	this->maxTransactionId = 0L;
	this->maxObjectId = 0L;
	this->maxSchemaObjectId = 0L;
	this->maxCommitId = 0L;
}

IdsBackupCommand::~IdsBackupCommand() {

}

void IdsBackupCommand::setValues(uint64_t maxTransactionId, uint64_t maxObjectId, uint64_t maxSchemaObjectId, uint64_t maxCommitId) noexcept {
	this->maxTransactionId =maxTransactionId;
	this->maxObjectId = maxObjectId;
	this->maxSchemaObjectId = maxSchemaObjectId;
	this->maxCommitId = maxCommitId;
}

int IdsBackupCommand::binarySize() const {
	int total = sizeof(uint8_t);

	total += sizeof(this->maxTransactionId);
	total += sizeof(this->maxObjectId);
	total += sizeof(this->maxSchemaObjectId);
	total += sizeof(this->maxCommitId);

	return total;
}

void IdsBackupCommand::toBinary(ByteBuffer *out) const {
	out->put(this->type);

	out->putLong(this->maxTransactionId);
	out->putLong(this->maxObjectId);
	out->putLong(this->maxSchemaObjectId);
	out->putLong(this->maxCommitId);
}

void IdsBackupCommand::fromBinary(ByteBuffer *in) {
	this->maxTransactionId = in->getLong();
	this->maxObjectId = in->getLong();
	this->maxSchemaObjectId = in->getLong();
	this->maxCommitId = in->getLong();
}

void IdsBackupCommand::recover(ISnapshotRecoverer* recoverer, CodableDatabase* db) {
	SchemaManager* scManager = db->getSchemaManager();

	scManager->recoverObjectIds(this->maxTransactionId, this->maxObjectId, this->maxSchemaObjectId, this->maxCommitId);
}

void IdsBackupCommand::__testCheckEquals(const AbstractSnapshotCommand *other) const {
	DatabaseEqualsChecker::checkIntEquals(this->type, other->getType());

	const IdsBackupCommand* cmd = dynamic_cast<const IdsBackupCommand*>(other);

	DatabaseEqualsChecker::checkLongEquals(this->maxTransactionId, cmd->maxTransactionId);
	DatabaseEqualsChecker::checkLongEquals(this->maxObjectId, cmd->maxObjectId);
	DatabaseEqualsChecker::checkLongEquals(this->maxSchemaObjectId, cmd->maxSchemaObjectId);
	DatabaseEqualsChecker::checkLongEquals(this->maxCommitId, cmd->maxCommitId);
}

} /* namespace codablecash */
