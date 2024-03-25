/*
 * CdbRecordKey.cpp
 *
 *  Created on: 2020/05/30
 *      Author: iizuka
 */

#include "schema_table/record/table_record_key/CdbRecordKey.h"

#include "btree/AbstractBtreeKey.h"

#include "base_io/ByteBuffer.h"

#include "schema_table/record/table_record/CdbKeyFactory.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"
namespace codablecash {

CdbRecordKey::CdbRecordKey(const CdbRecordKey& inst) : AbstractCdbKey(AbstractCdbKey::TYPE_RECORD_KEY) {
	int maxLoop = inst.list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractBtreeKey* key = inst.list.get(i);

		if(key == nullptr){
			this->list.addElement(nullptr);
			continue;
		}

		key = dynamic_cast<AbstractBtreeKey*>(key->clone());
		this->list.addElement(key);
	}
}

CdbRecordKey::CdbRecordKey() : AbstractCdbKey(AbstractCdbKey::TYPE_RECORD_KEY) {

}

CdbRecordKey::~CdbRecordKey() {
	this->list.deleteElements();
}

AbstractBtreeKey* CdbRecordKey::clone() const noexcept {
	return new CdbRecordKey(*this);
}

int CdbRecordKey::compareTo(const AbstractBtreeKey* key) const noexcept {
	if(key != nullptr && key->isInfinity()){
		return -1;
	}
	else if(key == nullptr || key->isNull()){
		return 1;
	}

	const CdbRecordKey* right = dynamic_cast<const CdbRecordKey*>(key);

	int result = 0;

	int leftSize = this->list.size();
	for(int i = 0; i != leftSize; ++i){
		AbstractBtreeKey* l = this->list.get(i);
		AbstractBtreeKey* r = right->list.get(i);

		result = l != nullptr ? l->compareTo(r) :
					(r == nullptr || r->isNull()) ? 0 : -1;

		if(result == 0){
			continue;
		}

		break;
	}

	return result;
}

int CdbRecordKey::binarySize() const {
	int total = sizeof(uint32_t);

	int maxLoop = this->list.size();
	total += sizeof(int32_t);

	for(int i = 0; i != maxLoop; ++i){
		AbstractBtreeKey* key = this->list.get(i);

		if(key == nullptr){
			total += sizeof(uint32_t);
			continue;
		}

		total += key->binarySize();
	}

	return total;
}

void CdbRecordKey::toBinary(ByteBuffer* out) const {
	out->putInt(this->type);

	int maxLoop = this->list.size();
	out->putInt(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		AbstractBtreeKey* key = this->list.get(i);

		if(key == nullptr){
			out->putInt(0);
			continue;
		}

		key->toBinary(out);
	}
}

void CdbRecordKey::fromBinary(ByteBuffer* in) {
	CdbKeyFactory factory;

	int maxLoop = in->getInt();
	for(int i = 0; i != maxLoop; ++i){
		uint32_t keyType = in->getInt();
		AbstractBtreeKey* abkey = factory.fromBinary(keyType, in);

		AbstractBtreeKey* key = dynamic_cast<AbstractBtreeKey*>(abkey);
		//assert(key != nullptr);

		addKey(key);
	}

}

void CdbRecordKey::addKey(AbstractBtreeKey* key) noexcept {
	//assert(key != nullptr);

	CdbRecordKey* recordKey = dynamic_cast<CdbRecordKey*>(key);
	if(recordKey != nullptr){
		addRecordKey(recordKey);
		delete recordKey;
		return;
	}

	this->list.addElement(key);
}

void CdbRecordKey::addRecordKey(CdbRecordKey* recordKey) noexcept {
	int maxLoop = recordKey->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractBtreeKey* key = recordKey->list.get(i);

		AbstractBtreeKey* key2 = key != nullptr ? key->clone() : nullptr;
		this->list.addElement(key2);
	}
}

AbstractCdbValue* CdbRecordKey::toCdbValue() const {
	CdbRecord* rec = new CdbRecord();

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractBtreeKey* key = this->list.get(i);
		const AbstractCdbKey* cdbKey = dynamic_cast<const AbstractCdbKey*>(key);

		rec->addValue(cdbKey != nullptr ? cdbKey->toCdbValue() : nullptr);
	}

	return rec;
}

} /* namespace codablecash */
