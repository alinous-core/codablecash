/*
 * CdbInfinityKey.cpp
 *
 *  Created on: 2022/01/01
 *      Author: iizuka
 */

#include "schema_table/record/table_record_key/CdbInfinityKey.h"

#include "schema_table/record/table_record_value/CdbInfinityValue.h"

#include "base_io/ByteBuffer.h"

namespace codablecash {

CdbInfinityKey::CdbInfinityKey(const CdbInfinityKey& inst) : AbstractCdbKey(AbstractCdbKey::TYPE_INFINITY) {

}

CdbInfinityKey::CdbInfinityKey() : AbstractCdbKey(AbstractCdbKey::TYPE_INFINITY) {

}

CdbInfinityKey::~CdbInfinityKey() {

}

AbstractBtreeKey* CdbInfinityKey::clone() const noexcept {
	return new CdbInfinityKey(*this);
}

int CdbInfinityKey::compareTo(const AbstractBtreeKey* key) const noexcept {
	if(key != nullptr && key->isInfinity()){
		return 0;
	}
	return 1;
}

bool CdbInfinityKey::isInfinity() const {
	return true;
}

AbstractCdbValue* CdbInfinityKey::toCdbValue() const {
	return new CdbInfinityValue();
}

int CdbInfinityKey::binarySize() const {
	return sizeof(uint32_t);
}

void CdbInfinityKey::toBinary(ByteBuffer* out) const {
	out->putInt(this->type);
}

void CdbInfinityKey::fromBinary(ByteBuffer* in) {
}

} /* namespace codablecash */
