/*
 * UndoIndexPart.h
 *
 *  Created on: 2023/07/16
 *      Author: iizuka
 */

#ifndef TRX_SESSION_DDL_UNDOINDEXPART_H_
#define TRX_SESSION_DDL_UNDOINDEXPART_H_

#include "base/ArrayList.h"

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class CdbOid;
class CdbTableColumn;

class UndoIndexPart {
public:
	UndoIndexPart(const UndoIndexPart& inst);
	UndoIndexPart();
	virtual ~UndoIndexPart();

	int binarySize() const;
	void toBinary(ByteBuffer* out) const;
	static UndoIndexPart* fromBinary(ByteBuffer* in);

	void setOid(const CdbOid* oid) noexcept;
	void setName(const UnicodeString* name) noexcept;
	void setPrimary(bool bl) noexcept;
	void setUnique(bool bl) noexcept;

	void addColumn(const CdbOid* col) noexcept;

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

private:
	CdbOid* oid;
	UnicodeString* name;
	bool primary;
	bool unique;

	ArrayList<CdbOid>* columns;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_DDL_UNDOINDEXPART_H_ */
