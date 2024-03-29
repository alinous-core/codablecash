/*
 * CdbByteValue.cpp
 *
 *  Created on: 2020/05/20
 *      Author: iizuka
 */

#include "schema_table/record/table_record_value/CdbByteValue.h"

#include "base_io/ByteBuffer.h"

#include "schema_table/record/table_record_key/CdbByteKey.h"

namespace codablecash {

CdbByteValue::CdbByteValue(const CdbByteValue& inst) : AbstractCdbValue(AbstractCdbValue::TYPE_BYTE) {
	this->value = inst.value;
}

CdbByteValue::CdbByteValue(int8_t value) : AbstractCdbValue(AbstractCdbValue::TYPE_BYTE) {
	this->value = value;
}

CdbByteValue::CdbByteValue() : AbstractCdbValue(AbstractCdbValue::TYPE_BYTE) {
	this->value = 0;
}

CdbByteValue::~CdbByteValue() {

}

int CdbByteValue::binarySize() const {
	int total = sizeof(int8_t);
	total += sizeof(int8_t);
	return total;
}

void CdbByteValue::toBinary(ByteBuffer* out) const {
	out->put(this->type);
	out->put(this->value);
}

void CdbByteValue::fromBinary(ByteBuffer* in) {
	this->value = in->get();
}

AbstractCdbValue* CdbByteValue::copy() const noexcept {
	return new CdbByteValue(*this);
}

AbstractCdbKey* CdbByteValue::toKey() const noexcept {
	return new CdbByteKey(this->value);
}

bool CdbByteValue::strictEqualsValue(const AbstractCdbValue *v) const {
	const CdbByteValue* other = dynamic_cast<const CdbByteValue*>(v);

	return other->value == this->value;
}

} /* namespace codablecash */
