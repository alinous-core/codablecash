/*
 * TransactionIdsListData.cpp
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#include "bc_base_trx_index/TransactionIdsListData.h"

#include "bc_trx/TransactionId.h"

#include "base/StackRelease.h"

namespace codablecash {

TransactionIdsListData::TransactionIdsListData() {
	this->index = 0;
}

TransactionIdsListData::~TransactionIdsListData() {
	this->list.deleteElements();
}

void TransactionIdsListData::join(const TransactionIdsListData *value) noexcept {
	const ArrayList<TransactionId>* newList = &value->list;

	int maxLoop = newList->size();
	for(int i = 0; i != maxLoop; ++i){
		TransactionId* v = newList->get(i);
		if(contains(v)){
			continue;
		}

		TransactionId* element = dynamic_cast<TransactionId*>(v->copyData());
		this->list.addElement(element);
	}
}

bool TransactionIdsListData::contains(const TransactionId *value) const noexcept {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		TransactionId* v = this->list.get(i);
		if(v->equals(value)){
			return true;
		}
	}

	return false;
}

void TransactionIdsListData::remove(const TransactionId *value) noexcept {
	int index = indexof(value);

	if(index >= 0){
		delete this->list.remove(index);
	}
}

int TransactionIdsListData::indexof(const TransactionId *value) const noexcept {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		TransactionId* v = this->list.get(i);
		if(v->equals(value)){
			return i;
		}
	}

	return -1;
}

bool TransactionIdsListData::isEmpty() const noexcept {
	return this->list.size() == 0;
}

int TransactionIdsListData::binarySize() const {
	int total = sizeof(uint16_t);

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		TransactionId* trxId = this->list.get(i);
		total += trxId ->binarySize();
	}

	return total;
}

void TransactionIdsListData::toBinary(ByteBuffer *out) const {
	int maxLoop = this->list.size();
	out->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		TransactionId* trxId = this->list.get(i);
		trxId->toBinary(out);
	}
}

TransactionIdsListData* TransactionIdsListData::fromBinary(ByteBuffer *in) {
	TransactionIdsListData* data = new TransactionIdsListData();

	int maxLoop = in->getShort();
	for(int i = 0; i != maxLoop; ++i){
		TransactionId* trxId = TransactionId::fromBinary(in); __STP(trxId);
		data->add(trxId);
	}

	return data;
}

IBlockObject* TransactionIdsListData::copyData() const noexcept {
	TransactionIdsListData * data = new TransactionIdsListData();

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		TransactionId* v = this->list.get(i);

		TransactionId* trxId = dynamic_cast<TransactionId*>(v->copyData()); __STP(trxId);
		data->add(trxId);
	}

	return data;
}

void TransactionIdsListData::add(const TransactionId *trxId) {
	this->list.addElement(dynamic_cast<TransactionId*>(trxId->copyData()));
}

} /* namespace codablecash */
