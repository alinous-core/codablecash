/*
 * TableEndSnapshotCommand.h
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#ifndef SNAPSHOT_TABLEENDSNAPSHOTCOMMAND_H_
#define SNAPSHOT_TABLEENDSNAPSHOTCOMMAND_H_

#include "snapshot/AbstractSnapshotCommand.h"

namespace codablecash {

class TableEndSnapshotCommand : public AbstractSnapshotCommand {
public:
	TableEndSnapshotCommand();
	virtual ~TableEndSnapshotCommand();

	virtual void recover(ISnapshotRecoverer* recoverer, CodableDatabase* db);

	virtual bool popStack(){return true;}

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual void __testCheckEquals(const AbstractSnapshotCommand* other) const;
};

} /* namespace codablecash */

#endif /* SNAPSHOT_TABLEENDSNAPSHOTCOMMAND_H_ */
