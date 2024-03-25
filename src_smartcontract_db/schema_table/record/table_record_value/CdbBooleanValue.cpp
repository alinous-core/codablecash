/*
 * CdbBooleanValue.cpp
 *
 *  Created on: 2021/12/06
 *      Author: iizuka
 */

#include "schema_table/record/table_record_value/CdbBooleanValue.h"

#include "schema_table/record/table_record_key/CdbBooleanKey.h"

#include "base_io/ByteBuffer.h"


namespace codablecash {

CdbBooleanValue::CdbBooleanValue(const CdbBooleanValue& inst) : AbstractCdbValue(AbstractCdbValue::TYPE_BOOLEAN) {
	this->value = inst.value;
}

CdbBooleanValue::CdbBooleanValue(bool bl) : AbstractCdbValue(AbstractCdbValue::TYPE_BOOLEAN) {
	this->value = bl;
}

CdbBooleanValue::CdbBooleanValue() : AbstractCdbValue(AbstractCdbValue::TYPE_BOOLEAN) {
	this->value = false;
}

CdbBooleanValue::~CdbBooleanValue() {

}

int CdbBooleanValue::binarySize() const {
	int total = sizeof(int8_t);
	total += sizeof(int8_t);
	return total;
}

void CdbBooleanValue::toBinary(ByteBuffer* out) const {
	out->put(this->type);
	out->put(this->value ? 1 : 0);
}

void CdbBooleanValue::fromBinary(ByteBuffer* in) {
	int8_t b = in->get();
	this->value = b > 0;
}

AbstractCdbValue* CdbBooleanValue::copy() const noexcept {
	return new CdbBooleanValue(*this);
}

AbstractCdbKey* CdbBooleanValue::toKey() const noexcept {
	return new CdbBooleanKey(this->value);
}

bool CdbBooleanValue::strictEqualsValue(const AbstractCdbValue *v) const {
	const CdbBooleanValue* other = dynamic_cast<const CdbBooleanValue*>(v);

	return other->value == this->value;
}

} /* namespace codablecash */
