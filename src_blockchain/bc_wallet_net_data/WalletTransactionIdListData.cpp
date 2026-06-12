/*
 * TransactionIdListData.cpp
 *
 *  Created on: Jun 9, 2026
 *      Author: iizuka
 */

#include "bc_wallet_net_data/WalletTransactionIdListData.h"

#include "bc_trx/TransactionId.h"

#include "base/StackRelease.h"

namespace codablecash {

WalletTransactionIdListData::WalletTransactionIdListData() {
	this->list = new ArrayList<TransactionId>();
	this->index = 0;
}

WalletTransactionIdListData::~WalletTransactionIdListData() {
	this->list->deleteElements();
	delete this->list;
}

void WalletTransactionIdListData::join(const WalletTransactionIdListData *value) noexcept {
	const ArrayList<TransactionId>* newList = value->list;

	int maxLoop = newList->size();
	for(int i = 0; i != maxLoop; ++i){
		TransactionId* v = newList->get(i);
		if(contains(v)){
			continue;
		}

		TransactionId* element = dynamic_cast<TransactionId*>(v->copyData());
		this->list->addElement(element);
	}
}

bool WalletTransactionIdListData::contains(const TransactionId *value) const noexcept {
	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		TransactionId* v = this->list->get(i);
		if(v->equals(value)){
			return true;
		}
	}

	return false;
}

void WalletTransactionIdListData::remove(const TransactionId *value) noexcept {
	int index = indexof(value);

	if(index >= 0){
		delete this->list->remove(index);
	}
}

int WalletTransactionIdListData::indexof(const TransactionId *value) const noexcept {
	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		TransactionId* v = this->list->get(i);
		if(v->equals(value)){
			return i;
		}
	}

	return -1;
}

bool WalletTransactionIdListData::isEmpty() const noexcept {
	return this->list->size() == 0;
}

IBlockObject* WalletTransactionIdListData::copyData() const noexcept {
	WalletTransactionIdListData * data = new WalletTransactionIdListData();

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		TransactionId* v = this->list->get(i);

		TransactionId* trxId = dynamic_cast<TransactionId*>(v->copyData()); __STP(trxId);
		data->add(trxId);
	}

	return data;
}

void WalletTransactionIdListData::add(const TransactionId *trxId) {
	this->list->addElement(dynamic_cast<TransactionId*>(trxId->copyData()));
}

int WalletTransactionIdListData::binarySize() const {
	int total = sizeof(uint16_t);

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		TransactionId* trxId = this->list->get(i);
		total += trxId ->binarySize();
	}

	return total;
}

void WalletTransactionIdListData::toBinary(ByteBuffer *out) const {
	int maxLoop = this->list->size();
	out->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		TransactionId* trxId = this->list->get(i);
		trxId->toBinary(out);
	}
}

WalletTransactionIdListData* WalletTransactionIdListData::fromBinary(ByteBuffer *in) {
	WalletTransactionIdListData* data = new WalletTransactionIdListData();

	int maxLoop = in->getShort();
	for(int i = 0; i != maxLoop; ++i){
		TransactionId* trxId = TransactionId::fromBinary(in); __STP(trxId);
		data->add(trxId);
	}

	return data;
}

} /* namespace codablecash */
