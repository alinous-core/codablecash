/*
 * SchemaStartSnapshotCommand.h
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#ifndef SNAPSHOT_SCHEMAENDSNAPSHOTCOMMAND_H_
#define SNAPSHOT_SCHEMAENDSNAPSHOTCOMMAND_H_

#include "snapshot/AbstractSnapshotCommand.h"

namespace codablecash {

class SchemaEndSnapshotCommand : public AbstractSnapshotCommand {
public:
	SchemaEndSnapshotCommand();
	virtual ~SchemaEndSnapshotCommand();

	virtual void recover(ISnapshotRecoverer* recoverer, CodableDatabase* db);

	virtual bool popStack(){return true;}

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual void __testCheckEquals(const AbstractSnapshotCommand* other) const;
};

} /* namespace codablecash */

#endif /* SNAPSHOT_SCHEMAENDSNAPSHOTCOMMAND_H_ */
