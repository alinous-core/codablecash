/*
 * CdbValueList.cpp
 *
 *  Created on: 2022/01/15
 *      Author: iizuka
 */

#include "schema_table/record/table_record_value/CdbValueList.h"

#include "base_io/ByteBuffer.h"

#include "schema_table/record/table_record/CdbDataFactory.h"

#include "filestore_block/IBlockObject.h"
namespace codablecash {


CdbValueList::CdbValueList(const CdbValueList& inst) : AbstractCdbValue(AbstractCdbValue::TYPE_VALUE_LIST) {
	int maxLoop = inst.size();
	for(int i = 0; i != maxLoop; ++i){
		const AbstractCdbValue* v = inst.get(i);

		addValue(v->copy());
	}
}

CdbValueList::CdbValueList() : AbstractCdbValue(AbstractCdbValue::TYPE_VALUE_LIST) {

}

CdbValueList::~CdbValueList() {
	this->list.deleteElements();
}

int CdbValueList::binarySize() const {
	int total = sizeof(int8_t);

	int maxLoop = this->list.size();
	total += sizeof(int32_t);

	for(int i = 0; i != maxLoop; ++i){
		const AbstractCdbValue* value = this->list.get(i);
		total += value->binarySize();
	}

	return total;
}

void CdbValueList::toBinary(ByteBuffer* out) const {
	out->put(this->type);

	int maxLoop = this->list.size();
	out->putInt(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		const AbstractCdbValue* value = this->list.get(i);

		value->toBinary(out);
	}
}

void CdbValueList::fromBinary(ByteBuffer* in) {
	CdbDataFactory f;

	int maxLoop = in->getInt();
	for(int i = 0; i != maxLoop; ++i){
		IBlockObject* obj = f.makeDataFromBinary(in);

		addValue(dynamic_cast<AbstractCdbValue*>(obj));
	}
}

AbstractCdbValue* CdbValueList::copy() const noexcept {
	return new CdbValueList(*this);
}

AbstractCdbKey* CdbValueList::toKey() const noexcept {
	return nullptr;
}

void CdbValueList::addValue(AbstractCdbValue* value) noexcept {
	this->list.addElement(value);
}

int CdbValueList::size() const noexcept {
	return this->list.size();
}

const AbstractCdbValue* CdbValueList::get(int i) const noexcept {
	return this->list.get(i);
}

bool CdbValueList::strictEqualsValue(const AbstractCdbValue *v) const {
	const CdbValueList* other = dynamic_cast<const CdbValueList*>(v);

	int maxLoop = other->list.size();
	if(maxLoop != this->list.size()){
		return false;
	}

	bool ret = true;
	for(int i = 0; i != maxLoop; ++i){
		AbstractCdbValue* otherValue = other->list.get(i);
		AbstractCdbValue* value = this->list.get(i);

		if(!otherValue->strictEquals(value)){
			ret = false;
			break;
		}
	}

	return ret;
}

} /* namespace codablecash */
