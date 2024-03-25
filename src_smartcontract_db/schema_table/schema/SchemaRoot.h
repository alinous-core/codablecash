/*
 * SchemaBinary.h
 *
 *  Created on: 2020/05/11
 *      Author: iizuka
 */

#ifndef SCHEMA_SCHEMAROOT_H_
#define SCHEMA_SCHEMAROOT_H_
#include <cstdint>

#include "base/ArrayList.h"
#include "base/HashMap.h"

#include "engine/CdbBinaryObject.h"
#include "schema_table/schema/SchemaCompare.h"

namespace alinous {
class UnicodeString;
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class Schema;
class CdbTable;
class ISnapshotHandler;
class CdbStorageManager;
class SchemaCompare;
class CdbOid;

class SchemaRoot : public CdbBinaryObject {
public:
	SchemaRoot();
	virtual ~SchemaRoot();

	void addSchemaName(const UnicodeString* name) noexcept;

	void addSchema(Schema* schema) noexcept;
	Schema* getSchema(const UnicodeString* name) const noexcept;

	int binarySize() const;
	void toBinary(ByteBuffer* out) const;
	void fromBinary(ByteBuffer* in);

	uint64_t newTransactionId() noexcept;
	uint64_t newSchemaObjectId() noexcept;
	uint64_t newRecordObjectId() noexcept;
	uint64_t newCommitId() noexcept;

	uint64_t getSchemaObjectVersionId() const noexcept;
	void upgradeSchemaObjectVersionId() noexcept;

	const CdbTable* createTable(const CdbTable* table);
	const CdbTable* recoverTable(const CdbTable* table);

	CdbTable* getTable(const CdbOid* tableOid) const;

	const ArrayList<Schema, SchemaCompare>* getSchemaList() const noexcept;
	void removeSchema(const UnicodeString *name) noexcept;

	void removeTable(const UnicodeString* schema, const UnicodeString* table);

	void snapshotObjectIds(ISnapshotHandler *handler) noexcept;
	void snapshot(ISnapshotHandler *handler, CdbStorageManager* storeManager);
	void recoverObjectIds(uint64_t maxTransactionId, uint64_t maxObjectId, uint64_t maxSchemaObjectId, uint64_t maxCommitId) noexcept;

	void __testCheckEqualsObjectIds(SchemaRoot* other);
	void __testCheckEquals(CdbStorageManager *storeManager, SchemaRoot *other, CdbStorageManager *otherStoreManager);

private:
	CdbTable* getTable(Schema* sc, const CdbOid *tableOid) const noexcept;

private:
	ArrayList<Schema, SchemaCompare> list;
	HashMap<UnicodeString, Schema> map;

	uint64_t maxTransactionId;
	uint64_t maxObjectId;
	uint64_t maxSchemaObjectId;
	uint64_t maxCommitId;

};

} /* namespace codablecash */

#endif /* SCHEMA_SCHEMAROOT_H_ */
