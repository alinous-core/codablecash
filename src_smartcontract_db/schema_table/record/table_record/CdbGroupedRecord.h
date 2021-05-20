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

class CdbGroupedRecord : public CdbRecord {
public:
	CdbGroupedRecord(const CdbGroupedRecord& inst);
	CdbGroupedRecord();
	virtual ~CdbGroupedRecord();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

private:
	ArrayList<CdbOid> oidlist;
};

} /* namespace codablecash */

#endif /* SCHEMA_TABLE_RECORD_TABLE_RECORD_CDBGROUPEDRECORD_H_ */
