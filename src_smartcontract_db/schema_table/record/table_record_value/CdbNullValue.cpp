/*
 * CdbNullValue.cpp
 *
 *  Created on: 2021/04/27
 *      Author: iizuka
 */

#include "schema_table/record/table_record_value/CdbNullValue.h"

#include "schema_table/record/table_record_key/CdbNullKey.h"

#include "base_io/ByteBuffer.h"


namespace codablecash {

CdbNullValue::CdbNullValue(const CdbNullValue& inst) : AbstractCdbValue(AbstractCdbValue::TYPE_NULL) {
}

CdbNullValue::CdbNullValue() : AbstractCdbValue(AbstractCdbValue::TYPE_NULL) {

}

CdbNullValue::~CdbNullValue() {

}

int CdbNullValue::binarySize() const {
	int total = sizeof(int8_t);

	return total;
}

void CdbNullValue::toBinary(ByteBuffer* out) const {
	out->put(this->type);
}

void CdbNullValue::fromBinary(ByteBuffer* in) {
}

AbstractCdbKey* CdbNullValue::toKey() const noexcept {
	return new CdbNullKey();
}

AbstractCdbValue* CdbNullValue::copy() const noexcept {
	return new CdbNullValue(*this);
}

} /* namespace codablecash */
