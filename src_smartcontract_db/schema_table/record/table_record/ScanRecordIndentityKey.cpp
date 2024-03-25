/*
 * ScanRecordIndentityKey.cpp
 *
 *  Created on: 2022/01/26
 *      Author: iizuka
 */

#include "schema_table/record/table_record/ScanRecordIndentityKey.h"
#include "schema_table/record/table_record/ScanRecordIndentity.h"

#include "base_io/ByteBuffer.h"

#include "schema_table/record/table_record/CdbKeyFactory.h"

#include "schema_table/record/table_record/CdbDataFactory.h"
namespace codablecash {

ScanRecordIndentityKey::ScanRecordIndentityKey(const ScanRecordIndentityKey& inst) : AbstractCdbKey(AbstractCdbKey::TYPE_RECORD_IDENTITY_KEY) {
	this->scanId = inst.scanId != nullptr ? new ScanRecordIndentity(*inst.scanId) : nullptr;
}


ScanRecordIndentityKey::ScanRecordIndentityKey() : AbstractCdbKey(AbstractCdbKey::TYPE_RECORD_IDENTITY_KEY) {
	this->scanId = new ScanRecordIndentity();
}

ScanRecordIndentityKey::~ScanRecordIndentityKey() {
	delete this->scanId;
}

int ScanRecordIndentityKey::compareTo(const AbstractBtreeKey* key) const noexcept {
	if(key != nullptr && key->isInfinity()){
		return -1;
	}
	else if(key == nullptr || key->isNull()){
		return 1;
	}

	const ScanRecordIndentityKey* right = dynamic_cast<const ScanRecordIndentityKey*>(key);
	assert(right != nullptr);

	return this->scanId->compareTo(right->scanId);
}

int ScanRecordIndentityKey::binarySize() const {
	int total = sizeof(uint32_t);

	total += this->scanId->binarySize();

	return total;
}

void ScanRecordIndentityKey::toBinary(ByteBuffer* out) const {
	out->putInt(this->type);

	this->scanId->toBinary(out);
}

void ScanRecordIndentityKey::fromBinary(ByteBuffer* in) {
	CdbDataFactory factory;

	IBlockObject* obj = factory.makeDataFromBinary(in);
	ScanRecordIndentity* id = dynamic_cast<ScanRecordIndentity*>(obj);

	setRecordScanId(id);
}

AbstractCdbValue* ScanRecordIndentityKey::toCdbValue() const {
	return new ScanRecordIndentity(*this->scanId);
}

AbstractBtreeKey* ScanRecordIndentityKey::clone() const noexcept {
	return new ScanRecordIndentityKey(*this);
}

int64_t ScanRecordIndentityKey::toInt64() const noexcept {
	return 0;
}

void ScanRecordIndentityKey::setRecordScanId(ScanRecordIndentity* scanId) noexcept {
	delete this->scanId;
	this->scanId = scanId;
}

} /* namespace codablecash */
