/*
 * LockInOperationData.cpp
 *
 *  Created on: Oct 18, 2024
 *      Author: iizuka
 */

#include "bc_status_cache_lockin/AbstractLockinOperation.h"

#include "base/StackRelease.h"
#include "bc_status_cache_lockin/LockInOperationsData.h"


namespace codablecash {

LockInOperationsData::LockInOperationsData() {

}

LockInOperationsData::~LockInOperationsData() {
	this->list.deleteElements();
}

int LockInOperationsData::binarySize() const {
	int total = sizeof(uint16_t);

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractLockinOperation* op = this->list.get(i);
		total += op->binarySize();
	}

	return total;
}

void LockInOperationsData::toBinary(ByteBuffer *out) const {
	int maxLoop = this->list.size();
	out->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		AbstractLockinOperation* op = this->list.get(i);
		op->toBinary(out);
	}
}

LockInOperationsData* LockInOperationsData::fromBinary(ByteBuffer *in) {
	LockInOperationsData* data = new LockInOperationsData();

	int maxLoop = in->getShort();
	for(int i = 0; i != maxLoop; ++i){
		AbstractLockinOperation* op = AbstractLockinOperation::createFromBinary(in); __STP(op);
		data->add(op);
	}

	return data;
}

IBlockObject* LockInOperationsData::copyData() const noexcept {
	LockInOperationsData * data = new LockInOperationsData();

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractLockinOperation* v = this->list.get(i);

		AbstractLockinOperation* op = dynamic_cast<AbstractLockinOperation*>(v->copyData()); __STP(op);
		data->add(op);
	}

	return data;
}

void LockInOperationsData::add(const AbstractLockinOperation *op) {
	this->list.addElement(dynamic_cast<AbstractLockinOperation*>(op->copyData()));
}

void LockInOperationsData::join(const LockInOperationsData *value) noexcept {
	const ArrayList<AbstractLockinOperation>* newList = &value->list;

	int maxLoop = newList->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractLockinOperation* v = newList->get(i);
		if(contains(v)){
			continue;
		}

		AbstractLockinOperation* element = dynamic_cast<AbstractLockinOperation*>(v->copyData());
		this->list.addElement(element);
	}
}

bool LockInOperationsData::contains(const AbstractLockinOperation *value) const noexcept {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractLockinOperation* v = this->list.get(i);
		if(v->equals(value)){
			return true;
		}
	}

	return false;
}

void LockInOperationsData::apply(const BlockHeader *header, IStatusCacheContext *context) {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractLockinOperation* v = this->list.get(i);

		v->apply(header, context);
	}
}

} /* namespace codablecash */
