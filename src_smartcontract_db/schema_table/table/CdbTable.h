/*
 * CdbTable.h
 *
 *  Created on: 2020/05/13
 *      Author: iizuka
 */

#ifndef TABLE_CDBTABLE_H_
#define TABLE_CDBTABLE_H_

#include <cstdint>

#include "base/ArrayList.h"
#include "base/HashMap.h"

#include "engine/CdbBinaryObject.h"

namespace alinous {
class ByteBuffer;
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class CdbTableColumn;
class CdbOid;
class SchemaObjectIdPublisher;
class CdbTableIndex;
class Schema;
class ScanResultMetadata;

class CdbTable : public CdbBinaryObject {
public:
	static const constexpr uint8_t CDB_OBJ_TYPE{1};

	CdbTable(const CdbTable& inst);

	explicit CdbTable(uint64_t oid);
	virtual ~CdbTable();

	const CdbOid* getOid() const noexcept {
		return this->oid;
	}

	void addColumn(uint8_t oid, const wchar_t* name, uint8_t type, int length, bool notnull, bool unique, const wchar_t* defaultValue) noexcept;
	void addColumn(uint8_t oid, const UnicodeString* name, uint8_t type, int length, bool notnull, bool unique, const UnicodeString* defaultValue) noexcept;
	void addColumn(CdbTableColumn* col) noexcept;

	CdbTableColumn* getColumn(const wchar_t* name) noexcept;
	CdbTableColumn* getColumn(const UnicodeString* name) const noexcept;
	CdbTableColumn* getColumn(int index) const noexcept;
	CdbTableColumn* getColumn(const CdbOid* columnOid) const noexcept;

	CdbTableColumn* findColumnByOid(const CdbOid* oid) const noexcept;
	const ArrayList<CdbTableColumn>* getColumns() const noexcept;

	void assignNewOid(SchemaObjectIdPublisher* publisher);
	void setupUniqueIndexes();
	void setOid(uint64_t oid) noexcept;

	void setName(UnicodeString* name) noexcept;
	const UnicodeString* getName() const noexcept {
		return this->name;
	}
	const UnicodeString* getTableFqn() noexcept;

	const UnicodeString* getSchemaName() const noexcept {
		return this->schemaName;
	}
	void setSchemaName(UnicodeString* schemaName) noexcept;

	void setPrimaryKey(const wchar_t* col);
	void setPrimaryKey(const UnicodeString* colstr);
	const CdbTableIndex* setPrimaryKeys(const ArrayList<UnicodeString>* cols);

	bool hasSinglePrimaryKeyColumn(const CdbOid* columnOid) const noexcept;
	CdbTableIndex* getPrimaryKey() const noexcept;

	void addIndex(CdbTableIndex* index);
	CdbTableIndex* getIndexByColumnOid(const CdbOid* oid) const noexcept;
	CdbTableIndex* getIndexByColumnOids(const ArrayList<const CdbOid>* oidlist) const noexcept;
	CdbTableIndex* getIndexByColumnOidsStrict(const ArrayList<const CdbOid>* oidlist, bool uniqueData) const noexcept;

	CdbTableIndex* findMostAvailableIndex(const ArrayList<const CdbOid>* oidlist) const noexcept;

	CdbTableIndex* getIndexByName(const UnicodeString* indexname) const noexcept;

	void removeIndex(const CdbTableIndex* ptr) noexcept;
	CdbTableColumn* removeColumn(const UnicodeString* columnName) noexcept;
	ArrayList<CdbTableIndex>* removeIndexesUsingColumn(const UnicodeString* columnName) noexcept;
	void renameColumn(const UnicodeString* lastColumn, const UnicodeString* newColumn);

	int binarySize() const;
	void toBinary(ByteBuffer* out) const;
	void fromBinary(ByteBuffer* in);

	void setSchema(Schema* schema) noexcept;
	const Schema* getSchema() const noexcept;

	const ArrayList<CdbTableIndex>* getIndexes() const noexcept;

	void adjustIndexColumnPosition() noexcept;

	ScanResultMetadata* getMetadata() const noexcept;
private:
	CdbOid* oid;

	UnicodeString* schemaName;
	UnicodeString* name;
	ArrayList<CdbTableColumn>* columns;
	HashMap<CdbOid, CdbTableColumn>* columnMap;

	ArrayList<CdbTableIndex>* indexes;

	Schema* parent;

	UnicodeString* fqn;
};

} /* namespace codablecash */

#endif /* TABLE_CDBTABLE_H_ */
