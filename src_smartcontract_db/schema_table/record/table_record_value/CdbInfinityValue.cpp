/*
 * CdbInfinityValue.cpp
 *
 *  Created on: 2022/01/01
 *      Author: iizuka
 */

#include "schema_table/record/table_record_value/CdbInfinityValue.h"

#include "base_io/ByteBuffer.h"

#include "schema_table/record/table_record_key/CdbInfinityKey.h"

namespace codablecash {

CdbInfinityValue::CdbInfinityValue(const CdbInfinityValue& inst) : AbstractCdbValue(AbstractCdbValue::TYPE_INFINITY) {

}

CdbInfinityValue::CdbInfinityValue() : AbstractCdbValue(AbstractCdbValue::TYPE_INFINITY) {

}

CdbInfinityValue::~CdbInfinityValue() {

}

int CdbInfinityValue::binarySize() const {
	int total = sizeof(int8_t);

	return total;
}

void CdbInfinityValue::toBinary(ByteBuffer* out) const {
	out->put(this->type);
}

void CdbInfinityValue::fromBinary(ByteBuffer* in) {
}

AbstractCdbKey* CdbInfinityValue::toKey() const noexcept {
	return new CdbInfinityKey();
}

AbstractCdbValue* CdbInfinityValue::copy() const noexcept {
	return new CdbInfinityValue(*this);
}

} /* namespace codablecash */
