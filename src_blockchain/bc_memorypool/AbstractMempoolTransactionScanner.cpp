/*
 * AbstractMempoolTransactionScanner.cpp
 *
 *  Created on: 2023/12/02
 *      Author: iizuka
 */

#include "bc_memorypool/AbstractMempoolTransactionScanner.h"
#include "bc_memorypool/MemPoolTransaction.h"
#include "bc_memorypool/MemoryPool.h"
#include "bc_memorypool/AbstractTrxMemoryPool.h"

#include "btree/BtreeReverseScanner.h"

#include "bc_base_trx_index/TransactionIdsListData.h"

namespace codablecash {

AbstractMempoolTransactionScanner::AbstractMempoolTransactionScanner(MemPoolTransaction* memTrx
		, BtreeReverseScanner* feeTrxScanner, AbstractTrxMemoryPool* trxPool) {
	this->memTrx = memTrx;
	this->trxPool = trxPool;
	this->feeTrxScanner = feeTrxScanner;
	this->listData = nullptr;
	this->trxId = nullptr;
}

AbstractMempoolTransactionScanner::~AbstractMempoolTransactionScanner() {
	this->memTrx = nullptr;
	delete this->feeTrxScanner;
}

void AbstractMempoolTransactionScanner::init() {
	this->feeTrxScanner->begin();

	if(this->feeTrxScanner->hasPrevious()){
		this->listData = dynamic_cast<TransactionIdsListData*>(this->feeTrxScanner->previous()->copyData());
	}
}

bool AbstractMempoolTransactionScanner::hasNext() {
	while(this->listData != nullptr){
		this->trxId = this->listData->next();

		if(this->trxId == nullptr){
			delete this->listData;
			this->listData = nullptr;

			if(this->feeTrxScanner->hasPrevious()){
				this->listData = dynamic_cast<TransactionIdsListData*>(this->feeTrxScanner->previous()->copyData());
			}

			continue;
		}

		if(isConsumed(this->trxId)){
			continue;
		}

		return true;
	}

	return false;
}

AbstractBlockchainTransaction* AbstractMempoolTransactionScanner::next() const {
	AbstractBlockchainTransaction* trx = this->trxPool->getBlockchainTransaction(this->trxId);
	return trx;
}

bool AbstractMempoolTransactionScanner::isConsumed(const TransactionId *trxId) const noexcept {
	return this->memTrx->isConsumed(trxId);
}

} /* namespace codablecash */
