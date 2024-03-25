/*
 * SnapshotIndexCommand.h
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#ifndef SNAPSHOT_SNAPSHOTINDEXCOMMAND_H_
#define SNAPSHOT_SNAPSHOTINDEXCOMMAND_H_

#include "snapshot/AbstractSnapshotCommand.h"

#include "base/ArrayList.h"
using alinous::ArrayList;

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class CdbOid;
class CdbTableColumn;

class SnapshotIndexCommand : public AbstractSnapshotCommand {
public:
	SnapshotIndexCommand();
	virtual ~SnapshotIndexCommand();

	void setOid(const CdbOid* oid) noexcept;
	void setName(const UnicodeString* name) noexcept;
	void setPrimary(bool b) noexcept;
	void setUnique(bool b) noexcept;

	void addColumn(const CdbTableColumn* col) noexcept;

	virtual void recover(ISnapshotRecoverer* recoverer, CodableDatabase* db);

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	const CdbOid* getOid() const noexcept {
		return this->oid;
	}
	const UnicodeString* getName() const noexcept {
		return this->name;
	}
	bool isPrimary() const noexcept {
		return this->primary;
	}
	bool isUnique() const noexcept {
		return this->unique;
	}
	const ArrayList<CdbOid>* getColumns() const noexcept {
		return this->columns;
	}

	virtual void __testCheckEquals(const AbstractSnapshotCommand* other) const;

private:
	CdbOid* oid;
	UnicodeString* name;
	bool primary;
	bool unique;

	ArrayList<CdbOid>* columns;
};

} /* namespace codablecash */

#endif /* SNAPSHOT_SNAPSHOTINDEXCOMMAND_H_ */
