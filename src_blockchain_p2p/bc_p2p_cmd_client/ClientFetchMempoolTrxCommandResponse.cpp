/*
 * ClientFetchMempoolTrxCommandResponse.cpp
 *
 *  Created on: Aug 17, 2025
 *      Author: iizuka
 */

#include "bc_p2p_cmd_client/ClientFetchMempoolTrxCommandResponse.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "bc_trx/AbstractBlockchainTransaction.h"

#include "bc_trx/TransactionId.h"

#include "numeric/BigInteger.h"
namespace codablecash {

ClientFetchMempoolTrxCommandResponse::ClientFetchMempoolTrxCommandResponse() : AbstractCommandResponse(TYPE_RES_CLIENT_FETCH_MEMPOOL_TRX) {
	this->list = new ArrayList<AbstractBlockchainTransaction>();
}

ClientFetchMempoolTrxCommandResponse::~ClientFetchMempoolTrxCommandResponse() {
	this->list->deleteElements();
	delete this->list;
}

int ClientFetchMempoolTrxCommandResponse::binarySize() const {
	int total = sizeof(this->type);

	int maxLoop = this->list->size();
	total += sizeof(uint32_t);

	for(int i = 0; i != maxLoop; ++i){
		const AbstractBlockchainTransaction* trx = this->list->get(i);

		total += trx->binarySize();
	}

	return total;
}


void ClientFetchMempoolTrxCommandResponse::toBinary(ByteBuffer *buff) const {
	buff->putInt(this->type);

	int maxLoop = this->list->size();
	buff->putInt(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		const AbstractBlockchainTransaction* trx = this->list->get(i);

		trx->toBinary(buff);
	}
}

void ClientFetchMempoolTrxCommandResponse::fromBinary(ByteBuffer *buff) {
	int maxLoop = buff->getInt();

	for(int i = 0; i != maxLoop; ++i){
		AbstractBlockchainTransaction* trx = AbstractBlockchainTransaction::createFromBinary(buff);

		this->list->addElement(trx);
	}
}

UnicodeString* ClientFetchMempoolTrxCommandResponse::toString() const noexcept {
	UnicodeString* ret = new UnicodeString(L""); __STP(ret);

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		const AbstractBlockchainTransaction* trx = this->list->get(i);
		const TransactionId* trxId = trx->getTransactionId();

		BigInteger* bigInt = trxId->toBigInteger(); __STP(bigInt);
		UnicodeString str = bigInt->toString(16);

		if(i > 0){
			ret->append(L", ");
		}
		ret->append(&str);
	}

	return __STP_MV(ret);
}

void ClientFetchMempoolTrxCommandResponse::addTransaction(const AbstractBlockchainTransaction *trx) {
	this->list->addElement(dynamic_cast<AbstractBlockchainTransaction*>(trx->copyData()));
}

} /* namespace codablecash */
