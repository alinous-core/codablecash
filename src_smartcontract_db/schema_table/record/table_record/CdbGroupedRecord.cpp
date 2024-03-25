/*
 * CdbGroupedRecord.cpp
 *
 *  Created on: 2021/05/20
 *      Author: iizuka
 */

#include "schema_table/record/table_record/CdbGroupedRecord.h"

#include "base_io/ByteBuffer.h"
#include "engine/CdbOid.h"

#include "schema_table/record/table_record_key/CdbRecordKey.h"

#include "trx/transaction_cache/GroupCache.h"

namespace codablecash {

CdbGroupedRecord::CdbGroupedRecord(const CdbGroupedRecord& inst) : CdbRecord(inst)  {
	this->groupCache = inst.groupCache;
	this->oidCache = inst.oidCache;
	this->orgCache = inst.orgCache;
}

CdbGroupedRecord::CdbGroupedRecord(const CdbRecord* record, GroupCache* groupCache, OidArrayCache* oidCache, OidKeyRecordCache* orgCache) : CdbRecord() {
	const ArrayList<AbstractCdbValue>* list = record->getValues();

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop;++i){
		const AbstractCdbValue* v = list->get(i);

		addValue(v->copy());
	}

	this->groupCache = groupCache;
	this->oidCache = oidCache;
	this->orgCache = orgCache;
}

CdbGroupedRecord::~CdbGroupedRecord() {
	this->groupCache = nullptr;
	this->oidCache = nullptr;
	this->orgCache = nullptr;
}

int CdbGroupedRecord::binarySize() const {
	int total = CdbRecord::binarySize();

	return total;
}

void CdbGroupedRecord::toBinary(ByteBuffer* out) const {
	CdbRecord::toBinary(out);
}

void CdbGroupedRecord::fromBinary(ByteBuffer* in) {
	CdbRecord::fromBinary(in);
}

uint64_t CdbGroupedRecord::getIndexOfCache() const noexcept {
	return this->groupCache->getRecordIndex(this);
}


} /* namespace codablecash */
