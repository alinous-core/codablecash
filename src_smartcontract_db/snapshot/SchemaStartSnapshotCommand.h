/*
 * SchemaEndSnapshotCommand.h
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#ifndef SNAPSHOT_SCHEMASTARTSNAPSHOTCOMMAND_H_
#define SNAPSHOT_SCHEMASTARTSNAPSHOTCOMMAND_H_

#include "snapshot/AbstractSnapshotCommand.h"

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class CdbOid;

class SchemaStartSnapshotCommand : public AbstractSnapshotCommand {
public:
	SchemaStartSnapshotCommand();
	virtual ~SchemaStartSnapshotCommand();

	void setOid(const CdbOid* oid) noexcept;
	void setName(const UnicodeString* name) noexcept;

	virtual void recover(ISnapshotRecoverer* recoverer, CodableDatabase* db);

	virtual bool pushStack(){return true;}

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	const UnicodeString* getName() const noexcept {
		return this->name;
	}

	virtual void __testCheckEquals(const AbstractSnapshotCommand* other) const;

private:
	CdbOid* oid;
	UnicodeString* name;
};

} /* namespace codablecash */

#endif /* SNAPSHOT_SCHEMASTARTSNAPSHOTCOMMAND_H_ */
