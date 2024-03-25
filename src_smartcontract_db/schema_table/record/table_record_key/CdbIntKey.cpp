/*
 * CdbIntKey.cpp
 *
 *  Created on: 2020/05/21
 *      Author: iizuka
 */

#include "schema_table/record/table_record_key/CdbIntKey.h"

#include "base_io/ByteBuffer.h"

#include "schema_table/record/table_record_value/CdbIntValue.h"

namespace codablecash {

CdbIntKey::CdbIntKey(const CdbIntKey& inst) : AbstractCdbKey(AbstractCdbKey::TYPE_INT) {
	this->value = inst.value;
}

CdbIntKey::CdbIntKey() : AbstractCdbKey(AbstractCdbKey::TYPE_INT) {
	this->value = 0;
}

CdbIntKey::CdbIntKey(int32_t value) : AbstractCdbKey(AbstractCdbKey::TYPE_INT) {
	this->value = value;
}

CdbIntKey::~CdbIntKey() {

}

AbstractBtreeKey* CdbIntKey::clone() const noexcept {
	return new CdbIntKey(*this);
}

int CdbIntKey::compareTo(const AbstractBtreeKey* key) const noexcept {
	if(key != nullptr && key->isInfinity()){
		return -1;
	}
	else if(key == nullptr || key->isNull()){
		return 1;
	}

	const AbstractCdbKey* cdbkey = dynamic_cast<const AbstractCdbKey*>(key);
	return (int64_t)this->value - cdbkey->toInt64();
}

int CdbIntKey::binarySize() const {
	return sizeof(uint32_t) + sizeof(this->value);
}

void CdbIntKey::toBinary(ByteBuffer* out) const {
	out->putInt(this->type);
	out->putInt(this->value);
}

void CdbIntKey::fromBinary(ByteBuffer* in) {
	this->value = in->getInt();
}

AbstractCdbValue* CdbIntKey::toCdbValue() const {
	return new CdbIntValue(this->value);
}

} /* namespace codablecash */
