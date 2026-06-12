/*
 * WalletMemoryPoolScanner.cpp
 *
 *  Created on: Jun 9, 2026
 *      Author: iizuka
 */

#include "bc_wallet_net_data/WalletMemoryPoolScanner.h"
#include "bc_wallet_net_data/WalletTransactionIdListData.h"
#include "bc_wallet_net_data/WalletMemoryPool.h"

#include "btree/BtreeScanner.h"


namespace codablecash {

WalletMemoryPoolScanner::WalletMemoryPoolScanner(const WalletMemoryPool* pool) {
	this->pool = pool;
	this->scanner = nullptr;
	this->data = nullptr;
	this->trxId = nullptr;
}

WalletMemoryPoolScanner::~WalletMemoryPoolScanner() {
	this->pool = nullptr;
	delete this->scanner;
	delete this->data;
}

void WalletMemoryPoolScanner::begin() {
	this->scanner = this->pool->getBtreeScanner();
	this->scanner->begin();
}

bool WalletMemoryPoolScanner::hasNext() {
	this->trxId = this->data != nullptr ? this->data->next() : nullptr;

	if(this->trxId == nullptr && this->scanner->hasNext()){
		const IBlockObject* obj = this->scanner->next();

		delete this->data;
		this->data = dynamic_cast<WalletTransactionIdListData*>(obj->copyData());

		this->trxId = this->data->next();
	}

	return this->trxId != nullptr;
}

AbstractBlockchainTransaction* WalletMemoryPoolScanner::next() {
	AbstractBlockchainTransaction* retTrx = nullptr;
	if(this->trxId != nullptr){
		retTrx = this->pool->getBlockchainTransaction(this->trxId);
	}

	return retTrx;
}

} /* namespace codablecash */
