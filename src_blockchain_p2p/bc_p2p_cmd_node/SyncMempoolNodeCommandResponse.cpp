/*
 * SyncMempoolNodeCommandResponse.cpp
 *
 *  Created on: 2023/11/03
 *      Author: iizuka
 */

#include "bc_p2p_cmd_node/SyncMempoolNodeCommandResponse.h"

#include "bc_trx/AbstractBlockchainTransaction.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "bc_base/BinaryUtils.h"


namespace codablecash {

SyncMempoolNodeCommandResponse::SyncMempoolNodeCommandResponse() : AbstractCommandResponse(TYPE_RES_SYNC_MEMPOOL) {
	this->list = new ArrayList<AbstractBlockchainTransaction>();
}

SyncMempoolNodeCommandResponse::~SyncMempoolNodeCommandResponse() {
	this->list->deleteElements();
	delete this->list;
}

int SyncMempoolNodeCommandResponse::binarySize() const {
	int total = sizeof(this->type);

	int maxLoop = this->list->size();
	total += sizeof(uint32_t);

	for(int i = 0; i != maxLoop; ++i){
		AbstractBlockchainTransaction* trx = this->list->get(i);

		total += trx->binarySize();
	}

	return total;
}

void SyncMempoolNodeCommandResponse::toBinary(ByteBuffer *buff) const {
	buff->putInt(this->type);

	int maxLoop = this->list->size();
	buff->putInt(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		AbstractBlockchainTransaction* trx = this->list->get(i);

		trx->toBinary(buff);
	}
}

void SyncMempoolNodeCommandResponse::fromBinary(ByteBuffer *buff) {
	int maxLoop = buff->getInt();
	for(int i = 0; i != maxLoop; ++i){
		AbstractBlockchainTransaction* trx = AbstractBlockchainTransaction::createFromBinary(buff);
		BinaryUtils::checkNotNull(trx);

		this->list->addElement(trx);
	}
}

UnicodeString* SyncMempoolNodeCommandResponse::toString() const noexcept {
	UnicodeString* str = new UnicodeString(L"[SyncMempoolNodeCommandResponse]");

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractBlockchainTransaction* trx = this->list->get(i);
		const TransactionId* trxId = trx->getTransactionId();

		UnicodeString* idstr = trxId->toString(); __STP(idstr);

		str->append(L" ");
		str->append(idstr);
	}

	return str;
}

void SyncMempoolNodeCommandResponse::addBlockchainTransaction(const AbstractBlockchainTransaction *trx) noexcept {
	this->list->addElement(dynamic_cast<AbstractBlockchainTransaction*>(trx->copyData()));
}

int SyncMempoolNodeCommandResponse::getResultCount() const noexcept {
	return this->list->size();
}

AbstractBlockchainTransaction* SyncMempoolNodeCommandResponse::get(int i) const noexcept {
	return this->list->get(i);
}

} /* namespace codablecash */
