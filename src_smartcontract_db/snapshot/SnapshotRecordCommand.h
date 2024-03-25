/*
 * SnapshotRecordCommand.h
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#ifndef SNAPSHOT_SNAPSHOTRECORDCOMMAND_H_
#define SNAPSHOT_SNAPSHOTRECORDCOMMAND_H_

#include "snapshot/AbstractSnapshotCommand.h"

namespace codablecash {

class CdbRecord;

class SnapshotRecordCommand : public AbstractSnapshotCommand {
public:
	SnapshotRecordCommand();
	virtual ~SnapshotRecordCommand();

	void setRecord(const CdbRecord* rec) noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual void recover(ISnapshotRecoverer* recoverer, CodableDatabase* db);

	const CdbRecord* getRecord() const noexcept {
		return this->record;
	}

	virtual void __testCheckEquals(const AbstractSnapshotCommand* other) const;

private:
	CdbRecord* record;
};

} /* namespace codablecash */

#endif /* SNAPSHOT_SNAPSHOTRECORDCOMMAND_H_ */
