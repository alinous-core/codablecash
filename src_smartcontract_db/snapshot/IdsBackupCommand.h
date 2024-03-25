/*
 * IdsBackupCommand.h
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#ifndef SNAPSHOT_IDSBACKUPCOMMAND_H_
#define SNAPSHOT_IDSBACKUPCOMMAND_H_

#include "snapshot/AbstractSnapshotCommand.h"

namespace codablecash {

class IdsBackupCommand : public AbstractSnapshotCommand {
public:
	IdsBackupCommand();
	virtual ~IdsBackupCommand();

	void setValues(uint64_t maxTransactionId, uint64_t maxObjectId, uint64_t maxSchemaObjectId, uint64_t maxCommitId) noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual void recover(ISnapshotRecoverer* recoverer, CodableDatabase* db);

	uint64_t getMaxCommitId() const noexcept {
		return this->maxCommitId;
	}

	uint64_t getMaxObjectId() const noexcept {
		return this->maxObjectId;
	}

	uint64_t getMaxSchemaObjectId() const noexcept {
		return this->maxSchemaObjectId;
	}

	uint64_t getMaxTransactionId() const noexcept {
		return this->maxTransactionId;
	}

	virtual void __testCheckEquals(const AbstractSnapshotCommand* other) const;

private:
	uint64_t maxTransactionId;
	uint64_t maxObjectId;
	uint64_t maxSchemaObjectId;
	uint64_t maxCommitId;
};

} /* namespace codablecash */

#endif /* SNAPSHOT_IDSBACKUPCOMMAND_H_ */
