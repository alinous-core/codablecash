/*
 * HeaderTransactionGroup.cpp
 *
 *  Created on: Apr 19, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_data/HeaderTransactionGroup.h"

#include "bc_block/BlockHeaderId.h"

#include "bc_trx/AbstractBlockchainTransaction.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "bc_trx/TransactionId.h"

namespace codablecash {

HeaderTransactionGroup::HeaderTransactionGroup(const HeaderTransactionGroup &inst) {
	this->headerId = inst.headerId != nullptr ? dynamic_cast<BlockHeaderId*>(inst.headerId->copyData()) : nullptr;

	this->list = new ArrayList<AbstractBlockchainTransaction>();

	int maxLoop = inst.list->size();
	for(int i = 0; i != maxLoop; ++i){
		const AbstractBlockchainTransaction* trx = inst.list->get(i);

		addTransaction(trx);
	}
}

HeaderTransactionGroup::HeaderTransactionGroup() {
	this->headerId = nullptr;
	this->list = new ArrayList<AbstractBlockchainTransaction>();
}

HeaderTransactionGroup::~HeaderTransactionGroup() {
	delete this->headerId;

	this->list->deleteElements();
	delete this->list;
}

int HeaderTransactionGroup::binarySize() const {
	BinaryUtils::checkNotNull(this->headerId);

	int total = 0;

	total += this->headerId->binarySize();

	int maxLoop = this->list->size();
	total += sizeof(uint16_t);

	for(int i = 0; i != maxLoop; ++i){
		AbstractBlockchainTransaction* trx = this->list->get(i);

		total += trx->binarySize();
	}

	return total;
}

void HeaderTransactionGroup::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->headerId);

	this->headerId->toBinary(out);

	int maxLoop = this->list->size();
	out->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		AbstractBlockchainTransaction* trx = this->list->get(i);

		trx->toBinary(out);
	}
}

HeaderTransactionGroup* HeaderTransactionGroup::createFromBinary(ByteBuffer* in) {
	HeaderTransactionGroup *group = new HeaderTransactionGroup(); __STP(group);

	BlockHeaderId* headerId = BlockHeaderId::fromBinary(in); __STP(headerId);
	BinaryUtils::checkNotNull(headerId);
	group->setHeaderId(headerId);

	int maxLoop = in->getShort();
	for(int i = 0; i != maxLoop; ++i){
		AbstractBlockchainTransaction* trx = AbstractBlockchainTransaction::createFromBinary(in); __STP(trx);

		group->addTransaction(trx);
	}

	return __STP_MV(group);
}

IBlockObject* HeaderTransactionGroup::copyData() const noexcept {
	return new HeaderTransactionGroup(*this);
}

void HeaderTransactionGroup::addTransaction(const AbstractBlockchainTransaction *trx) {
	AbstractBlockchainTransaction* t = dynamic_cast<AbstractBlockchainTransaction*>(trx->copyData());
	this->list->addElement(t);
}

void HeaderTransactionGroup::setHeaderId(const BlockHeaderId *headerId) {
	delete this->headerId;

	this->headerId = dynamic_cast<BlockHeaderId*>(headerId->copyData());
}

void HeaderTransactionGroup::join(const HeaderTransactionGroup *value) noexcept {
	int maxLoop = value->list->size();
	for(int i = 0; i != maxLoop; ++i){
		const AbstractBlockchainTransaction* v =  value->list->get(i);

		if(contains(v)){
			continue;
		}

		AbstractBlockchainTransaction* newTrx = dynamic_cast<AbstractBlockchainTransaction*>(v->copyData());
		this->list->addElement(newTrx);
	}
}

bool HeaderTransactionGroup::contains(const AbstractBlockchainTransaction *trx) const noexcept {
	const TransactionId* trxId = trx->getTransactionId();

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractBlockchainTransaction* v = this->list->get(i);
		const TransactionId* vId = v->getTransactionId();
		if(trxId->equals(vId)){
			return true;
		}
	}

	return false;
}

void HeaderTransactionGroup::remove(const TransactionId *trxId) noexcept {
	int index = indexof(trxId);

	if(index >= 0){
		this->list->remove(index);
	}
}

int HeaderTransactionGroup::indexof(const TransactionId *trxId) const noexcept {
	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractBlockchainTransaction* v = this->list->get(i);
		const TransactionId* id = v->getTransactionId();
		if(trxId->equals(id)){
			return i;
		}
	}

	return -1;
}

} /* namespace codablecash */
