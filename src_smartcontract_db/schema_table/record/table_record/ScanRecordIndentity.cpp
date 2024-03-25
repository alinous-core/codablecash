/*
 * ScanRecordIndentity.cpp
 *
 *  Created on: 2022/01/24
 *      Author: iizuka
 */

#include "schema_table/record/table_record/ScanRecordIndentity.h"
#include "schema_table/record/table_record/CdbDataFactory.h"

#include "engine/CdbOid.h"

#include "base_io/ByteBuffer.h"

#include "filestore_block/IBlockObject.h"

#include "schema_table/record/table_record/ScanRecordIndentityKey.h"

#include "base/StackRelease.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"
namespace codablecash {

ScanRecordIndentity::ScanRecordIndentity(const ScanRecordIndentity& inst) : AbstractCdbValue(AbstractCdbValue::TYPE_RECORD_IDENTITY) {
	int maxLoop = inst.list.size();
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* oid = inst.list.get(i);

		addOid(oid);
	}
}

ScanRecordIndentity::ScanRecordIndentity() : AbstractCdbValue(AbstractCdbValue::TYPE_RECORD_IDENTITY) {

}

ScanRecordIndentity::~ScanRecordIndentity() {
	this->list.deleteElements();
}

int ScanRecordIndentity::binarySize() const {
	int total = sizeof(int8_t);

	int maxLoop = this->list.size();
	total += sizeof(int32_t);

	for(int i = 0; i != maxLoop; ++i){
		CdbOid* oid = this->list.get(i);

		total += oid->binarySize();
	}

	return total;
}

void ScanRecordIndentity::toBinary(ByteBuffer* out) const {
	out->put(this->type);

	int maxLoop = this->list.size();
	out->putInt(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		CdbOid* oid = this->list.get(i);

		oid->toBinary(out);
	}
}

void ScanRecordIndentity::fromBinary(ByteBuffer* in) {
	CdbDataFactory factory;

	int maxLoop = in->getInt();

	for(int i = 0; i != maxLoop; ++i){
		CdbOid* oid = CdbOid::fromBinary(in); __STP(oid);

		addOid(oid);
	}
}

void ScanRecordIndentity::addOid(const CdbOid* oid) noexcept {
	this->list.addElement(oid->copy());
}

void ScanRecordIndentity::addOids(const ScanRecordIndentity* inst) noexcept {
	int maxLoop = inst->list.size();
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* o = inst->list.get(i);
		addOid(o);
	}
}

AbstractCdbValue* ScanRecordIndentity::copy() const noexcept {
	return new ScanRecordIndentity(*this);
}

AbstractCdbKey* ScanRecordIndentity::toKey() const noexcept {
	ScanRecordIndentityKey* key = new ScanRecordIndentityKey();
	key->setRecordScanId(new ScanRecordIndentity(*this));

	return key;
}

int ScanRecordIndentity::compareTo(const ScanRecordIndentity* right) const noexcept {
	int maxLoop = this->list.size();
	int maxLoopRight = right->list.size();

	if(maxLoop != maxLoopRight){
		return maxLoop - maxLoopRight;
	}

	int result = 0;

	for(int i = 0; i != maxLoop; ++i){
		AbstractCdbKey* l = this->list.get(i)->toKey(); __STP(l);
		AbstractCdbKey* r = right->list.get(i)->toKey(); __STP(r);

		result = l != nullptr ? l->compareTo(r) : l - r;

		if(result == 0){
			continue;
		}

		break;
	}

	return result;
}

bool ScanRecordIndentity::strictEqualsValue(const AbstractCdbValue *v) const {
	const ScanRecordIndentity* other = dynamic_cast<const ScanRecordIndentity*>(v);

	int maxLoop = other->list.size();
	if(maxLoop != this->list.size()){
		return false;
	}

	bool ret = true;
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* otherValue = other->list.get(i);
		CdbOid* value = this->list.get(i);

		if(!otherValue->equals(value)){
			ret = false;
			break;
		}
	}

	return ret;
}

} /* namespace codablecash */
