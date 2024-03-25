/*
 * TransferedDataIdList.cpp
 *
 *  Created on: 2023/09/19
 *      Author: iizuka
 */

#include "data_history/TransferedDataIdList.h"
#include "data_history/TransferedDataId.h"

#include "base/StackRelease.h"
namespace codablecash {

TransferedDataIdList::TransferedDataIdList(const TransferedDataIdList &inst) {
	this->list = new ArrayList<TransferedDataId>();

	int maxLoop = inst.list->size();
	for(int i = 0; i != maxLoop; ++i){
		TransferedDataId* dataId = inst.list->get(i);

		add(dataId);
	}
}

TransferedDataIdList::TransferedDataIdList() {
	this->list = new ArrayList<TransferedDataId>();
}

TransferedDataIdList::~TransferedDataIdList() {
	this->list->deleteElements();
	delete this->list;
}

int TransferedDataIdList::binarySize() const {
	int total = sizeof(uint16_t);

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		TransferedDataId* dataId = this->list->get(i);
		total += dataId->binarySize();
	}

	return total;
}

void TransferedDataIdList::toBinary(ByteBuffer *out) const {
	int maxLoop = this->list->size();
	out->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		TransferedDataId* dataId = this->list->get(i);
		dataId->toBinary(out);
	}

}

TransferedDataIdList* TransferedDataIdList::fromBinary(ByteBuffer *in) {
	TransferedDataIdList* data = new TransferedDataIdList();

	int maxLoop = in->getShort();
	for(int i = 0; i != maxLoop; ++i){
		TransferedDataId* dataId = TransferedDataId::fromBinary(in); __STP(dataId);
		data->add(dataId);
	}

	return data;
}

IBlockObject* TransferedDataIdList::copyData() const noexcept {
	return new TransferedDataIdList(*this);
}

void TransferedDataIdList::add(const TransferedDataId *dataId) noexcept {
	this->list->addElement(dynamic_cast<TransferedDataId*>(dataId->copyData()));
}

void TransferedDataIdList::join(const TransferedDataIdList *iDlist) noexcept {
	const ArrayList<TransferedDataId>* newList = iDlist->list;

	int maxLoop = newList->size();
	for(int i = 0; i != maxLoop; ++i){
		TransferedDataId* v = newList->get(i);
		if(contains(v)){
			continue;
		}

		TransferedDataId* element = dynamic_cast<TransferedDataId*>(v->copyData());
		this->list->addElement(element);
	}
}

bool TransferedDataIdList::contains(const TransferedDataId *value) const noexcept {
	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		TransferedDataId* v = this->list->get(i);
		if(v->equals(value)){
			return true;
		}
	}

	return false;
}

void TransferedDataIdList::remove(const TransferedDataId *value) noexcept {
	int index = indexof(value);

	if(index >= 0){
		delete this->list->remove(index);
	}
}

int TransferedDataIdList::indexof(const TransferedDataId *value) const noexcept {
	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		TransferedDataId* v = this->list->get(i);
		if(v->equals(value)){
			return i;
		}
	}

	return -1;
}

bool TransferedDataIdList::isEmpty() const noexcept {
	return this->list->isEmpty();
}

} /* namespace codablecash */
