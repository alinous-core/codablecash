/*
 * CdbTableIndex.h
 *
 *  Created on: 2020/05/13
 *      Author: iizuka
 */

#ifndef TABLE_CDBTABLEINDEX_H_
#define TABLE_CDBTABLEINDEX_H_
#include <cstdint>

#include "base/ArrayList.h"
#include "base/HashMap.h"

#include "engine/CdbBinaryObject.h"

using namespace alinous;

namespace codablecash {

class CdbOid;
class CdbTable;
class CdbTableColumn;
class SchemaObjectIdPublisher;
class CdbStorageManager;

class CdbTableIndex : public CdbBinaryObject {
public:
	static const constexpr uint8_t CDB_OBJ_TYPE{3};

	CdbTableIndex(const CdbTableIndex& inst);
	explicit CdbTableIndex(uint64_t oid);
	virtual ~CdbTableIndex();

	static UnicodeString* createPrimaryKeyIndexName(CdbTableIndex* index, CdbTable* table) noexcept;
	static UnicodeString* createUniqueKeyIndexName(CdbTable* table, const UnicodeString* colName) noexcept;
	void setName(UnicodeString* name) noexcept;

	void assignNewOid(SchemaObjectIdPublisher* publisher);
	void syncColumnOid(const CdbTable* table, const CdbOid *lastOid, const CdbOid *newOid);

	void setOid(uint64_t oid) noexcept;
	const CdbOid* getOid() const noexcept {
		return this->oid;
	}

	bool hasColumnOid(const CdbOid* colOid) const noexcept;
	int getColumnCoverCount(const ArrayList<const CdbOid>* oidlist) const noexcept;

	void addColumn(const CdbOid* colId) noexcept;

	void setPrimaryKey(bool bl);
	bool isPrimaryKey() const noexcept;
	void setUnique(bool unique) noexcept;
	bool isUnique() const noexcept;

	bool isUniqueDataRequired() const noexcept {
		return isPrimaryKey() || isUnique();
	}

	int binarySize() const;
	void toBinary(ByteBuffer* out) const;
	void fromBinary(ByteBuffer* in, CdbTable* table);

	const ArrayList<CdbOid>* getColumns() const noexcept {
		return this->columns;
	}
	int getColumnLength() const noexcept;

	const UnicodeString* getName() const noexcept {
		return this->name;
	}

	const CdbOid* getColumnAt(int idxColumnPos) const noexcept;

	void __testCheckEquals(CdbStorageManager *storeManager, CdbTableIndex *other, CdbStorageManager *otherStoreManager);

private:
	bool hasOid(const ArrayList<const CdbOid>* oidlist, const CdbOid* oid) const noexcept;

private:
	CdbOid* oid;

	UnicodeString* name;
	bool primary;
	bool unique;

	ArrayList<CdbOid>* columns;
};

} /* namespace codablecash */

#endif /* TABLE_CDBTABLEINDEX_H_ */
