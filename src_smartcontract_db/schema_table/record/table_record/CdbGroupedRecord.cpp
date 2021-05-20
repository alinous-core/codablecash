/*
 * CdbGroupedRecord.cpp
 *
 *  Created on: 2021/05/20
 *      Author: iizuka
 */

#include "schema_table/record/table_record/CdbGroupedRecord.h"

#include "base_io/ByteBuffer.h"
#include "engine/CdbOid.h"

namespace codablecash {

CdbGroupedRecord::CdbGroupedRecord(const CdbGroupedRecord& inst) : CdbRecord(inst)  {
	int maxLoop = inst.oidlist.size();
	for(int i = 0; i != maxLoop; ++i){

	}
}

CdbGroupedRecord::CdbGroupedRecord() {
	// TODO Auto-generated constructor stub

}

CdbGroupedRecord::~CdbGroupedRecord() {
	// TODO Auto-generated destructor stub
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

} /* namespace codablecash */
