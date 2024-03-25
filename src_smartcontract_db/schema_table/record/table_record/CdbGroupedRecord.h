/*
 * CdbGroupedRecord.h
 *
 *  Created on: 2021/05/20
 *      Author: iizuka
 */

#ifndef SCHEMA_TABLE_RECORD_TABLE_RECORD_CDBGROUPEDRECORD_H_
#define SCHEMA_TABLE_RECORD_TABLE_RECORD_CDBGROUPEDRECORD_H_

#include "schema_table/record/table_record/CdbRecord.h"

#include "base/ArrayList.h"

namespace codablecash {

class GroupCache;
class CdbRecordKey;
class OidArrayCache;
class OidKeyRecordCache;

class CdbGroupedRecord : public CdbRecord {
public:
	CdbGroupedRecord(const CdbGroupedRecord& inst);
	CdbGroupedRecord(const CdbRecord* record, GroupCache* groupCache, OidArrayCache* oidCache, OidKeyRecordCache* orgCache);
	virtual ~CdbGroupedRecord();

	virtual bool isGrouped() const noexcept {
		return true;
	}

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	OidArrayCache* getOidCache() const noexcept {
		return this->oidCache;
	}
	OidKeyRecordCache* getOidKeyRecordCache() const noexcept {
		return this->orgCache;
	}

	uint64_t getIndexOfCache() const noexcept;

private:
	GroupCache* groupCache;
	OidArrayCache* oidCache;
	OidKeyRecordCache* orgCache;
};

} /* namespace codablecash */

#endif /* SCHEMA_TABLE_RECORD_TABLE_RECORD_CDBGROUPEDRECORD_H_ */
