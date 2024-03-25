/*
 * TableStartSnapshotCommand.h
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#ifndef SNAPSHOT_TABLESTARTSNAPSHOTCOMMAND_H_
#define SNAPSHOT_TABLESTARTSNAPSHOTCOMMAND_H_

#include "snapshot/AbstractSnapshotCommand.h"

#include "base/ArrayList.h"

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class CdbOid;
class CdbTableColumn;
class CdbTable;

class TableStartSnapshotCommand : public AbstractSnapshotCommand {
public:
	TableStartSnapshotCommand();
	virtual ~TableStartSnapshotCommand();

	void setOid(const CdbOid* oid) noexcept;
	void setName(const UnicodeString* name) noexcept;
	void addColumn(const CdbTableColumn* col) noexcept;

	virtual void recover(ISnapshotRecoverer* recoverer, CodableDatabase* db);

	virtual bool pushStack(){return true;}

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* out);

	const CdbOid* getOid() const noexcept {
		return this->oid;
	}
	const UnicodeString* getName() const noexcept {
		return this->name;
	}
	CdbTable* getTable() const noexcept {
		return this->table;
	}

	const ArrayList<CdbTableColumn>* getColumns() const noexcept {
		return this->columns;
	}

	virtual void __testCheckEquals(const AbstractSnapshotCommand* other) const;

private:
	CdbOid* oid;
	UnicodeString* name;

	ArrayList<CdbTableColumn>* columns;

private:
	CdbTable* table;
};

} /* namespace codablecash */

#endif /* SNAPSHOT_TABLESTARTSNAPSHOTCOMMAND_H_ */
