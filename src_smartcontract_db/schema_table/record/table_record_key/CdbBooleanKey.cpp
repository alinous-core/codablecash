/*
 * CdbBooleanKey.cpp
 *
 *  Created on: 2021/12/06
 *      Author: iizuka
 */

#include "schema_table/record/table_record_key/CdbBooleanKey.h"

#include "schema_table/record/table_record_value/CdbBooleanValue.h"

#include "base_io/ByteBuffer.h"

namespace codablecash {

CdbBooleanKey::CdbBooleanKey(const CdbBooleanKey& inst) : AbstractCdbKey(AbstractCdbKey::TYPE_BOOLEAN) {
	this->value = inst.value;
}

CdbBooleanKey::CdbBooleanKey(bool value) : AbstractCdbKey(AbstractCdbKey::TYPE_BOOLEAN) {
	this->value = value;
}

CdbBooleanKey::CdbBooleanKey() : AbstractCdbKey(AbstractCdbKey::TYPE_BOOLEAN) {
	this->value = false;
}

CdbBooleanKey::~CdbBooleanKey() {
}

AbstractBtreeKey* CdbBooleanKey::clone() const noexcept {
	return new CdbBooleanKey(*this);
}

int CdbBooleanKey::compareTo(const AbstractBtreeKey* key) const noexcept {
	if(key != nullptr && key->isInfinity()){
		return -1;
	}
	else if(key == nullptr || key->isNull()){
		return 1;
	}

	const AbstractCdbKey* cdbkey = dynamic_cast<const AbstractCdbKey*>(key);
	return (int64_t)toInt64() - cdbkey->toInt64();
}

AbstractCdbValue* CdbBooleanKey::toCdbValue() const {
	return new CdbBooleanValue(this->value);
}

int CdbBooleanKey::binarySize() const {
	return sizeof(uint32_t) + sizeof(int8_t);
}

void CdbBooleanKey::toBinary(ByteBuffer* out) const {
	out->putInt(this->type);
	out->put(this->value ? 1 : 0);
}

void CdbBooleanKey::fromBinary(ByteBuffer* in) {
	int8_t b = in->get();
	this->value = b > 0;
}


} /* namespace codablecash */
