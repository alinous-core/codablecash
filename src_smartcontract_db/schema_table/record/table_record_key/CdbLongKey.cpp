/*
 * CdbLongKey.cpp
 *
 *  Created on: 2020/05/21
 *      Author: iizuka
 */

#include "schema_table/record/table_record_key/CdbLongKey.h"

#include "base_io/ByteBuffer.h"

#include "schema_table/record/table_record_value/CdbLongValue.h"
namespace codablecash {

CdbLongKey::CdbLongKey(const CdbLongKey& inst) : AbstractCdbKey(AbstractCdbKey::TYPE_LONG) {
	this->value = inst.value;
}

CdbLongKey::CdbLongKey() : AbstractCdbKey(AbstractCdbKey::TYPE_LONG) {
	this->value = 0;
}

CdbLongKey::CdbLongKey(int64_t value) : AbstractCdbKey(AbstractCdbKey::TYPE_LONG) {
	this->value = value;
}


CdbLongKey::~CdbLongKey() {

}

AbstractBtreeKey* CdbLongKey::clone() const noexcept {
	return new CdbLongKey(*this);
}

int CdbLongKey::compareTo(const AbstractBtreeKey* key) const noexcept {
	if(key != nullptr && key->isInfinity()){
		return -1;
	}
	else if(key == nullptr || key->isNull()){
		return 1;
	}

	const AbstractCdbKey* cdbkey = dynamic_cast<const AbstractCdbKey*>(key);
	return (int64_t)this->value - cdbkey->toInt64();
}

int CdbLongKey::binarySize() const {
	return sizeof(uint32_t) + sizeof(this->value);
}

void CdbLongKey::toBinary(ByteBuffer* out) const {
	out->putInt(this->type);
	out->putLong(this->value);
}

void CdbLongKey::fromBinary(ByteBuffer* in) {
	this->value = in->getLong();
}

AbstractCdbValue* CdbLongKey::toCdbValue() const {
	return new CdbLongValue(this->value);
}

} /* namespace codablecash */
