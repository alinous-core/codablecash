/*
 * CdbNullKey.cpp
 *
 *  Created on: 2021/04/27
 *      Author: iizuka
 */

#include "schema_table/record/table_record_key/CdbNullKey.h"

#include "base_io/ByteBuffer.h"

#include "schema_table/record/table_record_value/CdbNullValue.h"
namespace codablecash {

CdbNullKey::CdbNullKey(const CdbNullKey& inst) : AbstractCdbKey(AbstractCdbKey::TYPE_NULL) {
}

CdbNullKey::CdbNullKey() : AbstractCdbKey(AbstractCdbKey::TYPE_NULL) {

}

CdbNullKey::~CdbNullKey() {
}

bool CdbNullKey::isNull() const {
	return true;
}

AbstractBtreeKey* CdbNullKey::clone() const noexcept {
	return new CdbNullKey(*this);
}

int CdbNullKey::compareTo(const AbstractBtreeKey* key) const noexcept {
	if(key == nullptr || key->isNull()){
		return 0;
	}
	return -1;
}

int CdbNullKey::binarySize() const {
	return sizeof(uint32_t);
}

void CdbNullKey::toBinary(ByteBuffer* out) const {
	out->putInt(this->type);
}

void CdbNullKey::fromBinary(ByteBuffer* in) {
}

AbstractCdbValue* CdbNullKey::toCdbValue() const {
	return new CdbNullValue();
}

} /* namespace codablecash */
