/*
 * AddressDescriptorUtxoData.cpp
 *
 *  Created on: 2023/02/23
 *      Author: iizuka
 */

#include "bc_base_utxo_index/AddressDescriptorUtxoData.h"

#include "bc_trx/AbstractUtxo.h"

#include "bc_base/BalanceUnit.h"

#include "base/StackRelease.h"

namespace codablecash {

AddressDescriptorUtxoData::AddressDescriptorUtxoData() {
}

AddressDescriptorUtxoData::~AddressDescriptorUtxoData() {
	this->list.deleteElements();
}

int AddressDescriptorUtxoData::binarySize() const {
	int total = sizeof(uint16_t);

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxo* utxo = this->list.get(i);
		total += utxo->binarySize();
	}

	return total;
}

void AddressDescriptorUtxoData::toBinary(ByteBuffer *out) const {
	int maxLoop = this->list.size();
	out->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxo* utxo = this->list.get(i);
		utxo->toBinary(out);
	}
}

AddressDescriptorUtxoData* AddressDescriptorUtxoData::fromBinary(ByteBuffer *in) {
	AddressDescriptorUtxoData* data = new AddressDescriptorUtxoData();

	int maxLoop = in->getShort();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxo* utxo = AbstractUtxo::createFromBinary(in); __STP(utxo);
		data->add(utxo);
	}

	return data;
}

void AddressDescriptorUtxoData::join(const AddressDescriptorUtxoData *value) noexcept {
	const ArrayList<AbstractUtxo>* newList = &value->list;

	int maxLoop = newList->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxo* v = newList->get(i);
		if(contains(v)){
			continue;
		}

		AbstractUtxo* element = dynamic_cast<AbstractUtxo*>(v->copyData());
		this->list.addElement(element);
	}
}

bool AddressDescriptorUtxoData::contains(const AbstractUtxo *value) const noexcept {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxo* v = this->list.get(i);
		if(v->equals(value)){
			return true;
		}
	}

	return false;
}

void AddressDescriptorUtxoData::remove(const AbstractUtxo *value) noexcept {
	int index = indexof(value);

	if(index >= 0){
		delete this->list.remove(index);
	}
}

int AddressDescriptorUtxoData::indexof(const AbstractUtxo *value) const noexcept {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxo* v = this->list.get(i);
		if(v->equals(value)){
			return i;
		}
	}

	return -1;
}

bool AddressDescriptorUtxoData::isEmpty() const noexcept {
	return this->list.size() == 0;
}

IBlockObject* AddressDescriptorUtxoData::copyData() const noexcept {
	AddressDescriptorUtxoData * data = new AddressDescriptorUtxoData();

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxo* v = this->list.get(i);

		AbstractUtxo* utxo = dynamic_cast<AbstractUtxo*>(v->copyData()); __STP(utxo);
		data->add(utxo);
	}

	return data;
}

void AddressDescriptorUtxoData::add(const AbstractUtxo *utxo) {
	this->list.addElement(dynamic_cast<AbstractUtxo*>(utxo->copyData()));
}

BalanceUnit AddressDescriptorUtxoData::getTotalAmount() const noexcept {
	BalanceUnit ret(0L);

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractUtxo* v = this->list.get(i);
		BalanceUnit am = v->getAmount();
		ret = ret + am;
	}

	return ret;
}

} /* namespace codablecash */
