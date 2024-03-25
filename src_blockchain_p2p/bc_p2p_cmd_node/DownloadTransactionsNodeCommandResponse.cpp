/*
 * DownloadTransactionsNodeCommandResponse.cpp
 *
 *  Created on: 2023/11/27
 *      Author: iizuka
 */

#include "bc_p2p_cmd_node/DownloadTransactionsNodeCommandResponse.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "bc_trx/TransactionId.h"
#include "bc_trx/AbstractBlockchainTransaction.h"

#include "bc_base/BinaryUtils.h"


namespace codablecash {

DownloadTransactionsNodeCommandResponse::DownloadTransactionsNodeCommandResponse() : AbstractCommandResponse(TYPE_RES_DOWNLOAD_TRANSACTIONS) {
	this->list = new ArrayList<AbstractBlockchainTransaction>();
}

DownloadTransactionsNodeCommandResponse::~DownloadTransactionsNodeCommandResponse() {
	this->list->deleteElements();
	delete this->list;
}
int DownloadTransactionsNodeCommandResponse::binarySize() const {
	int total = sizeof(this->type);

	int maxLoop = this->list->size();
	total += sizeof(uint16_t);

	for(int i = 0; i != maxLoop; ++i){
		const AbstractBlockchainTransaction* trx = this->list->get(i);

		total += trx->binarySize();
	}

	return total;
}

void DownloadTransactionsNodeCommandResponse::toBinary(ByteBuffer *buff) const {
	buff->putInt(this->type);

	int maxLoop = this->list->size();
	buff->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		const AbstractBlockchainTransaction* trx = this->list->get(i);

		trx->toBinary(buff);
	}
}

void DownloadTransactionsNodeCommandResponse::fromBinary(ByteBuffer *buff) {
	int maxLoop = buff->getShort();
	for(int i = 0; i != maxLoop; ++i){
		AbstractBlockchainTransaction* trx = AbstractBlockchainTransaction::createFromBinary(buff);
		BinaryUtils::checkNotNull(trx);

		this->list->addElement(trx);
	}
}

UnicodeString* DownloadTransactionsNodeCommandResponse::toString() const noexcept {
	UnicodeString* ret = new UnicodeString(L"Download Transactions Response : "); __STP(ret);

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		const AbstractBlockchainTransaction* trx = this->list->get(i);
		const TransactionId* id = trx->getTransactionId();

		if(i != 0){
			ret->append(L", ");
		}

		UnicodeString* idstr = id->toString(); __STP(idstr);
		ret->append(idstr);
	}

	return __STP_MV(ret);
}

void DownloadTransactionsNodeCommandResponse::addTransaction(const AbstractBlockchainTransaction *trx) {
	this->list->addElement(dynamic_cast<AbstractBlockchainTransaction*>(trx->copyData()));
}

int DownloadTransactionsNodeCommandResponse::size() const noexcept {
	return this->list->size();
}

AbstractBlockchainTransaction* DownloadTransactionsNodeCommandResponse::get(int i) const noexcept {
	return this->list->get(i);
}

} /* namespace codablecash */
