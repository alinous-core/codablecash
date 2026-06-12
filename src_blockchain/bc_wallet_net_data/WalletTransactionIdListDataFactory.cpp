/*
 * WalletTransactionIdListDataFactory.cpp
 *
 *  Created on: Jun 9, 2026
 *      Author: iizuka
 */

#include "bc_wallet_net_data/WalletTransactionIdListDataFactory.h"
#include "bc_wallet_net_data/WalletTransactionIdListData.h"

#include "base_io/ByteBuffer.h"

#include "btree/DataNode.h"
#include "btree/BtreeStorage.h"

#include "base/StackRelease.h"

#include "base_timestamp/SystemTimestampKey.h"

#include "bc_trx/TransactionId.h"

namespace codablecash {

WalletTransactionIdListDataFactory::WalletTransactionIdListDataFactory() {

}

WalletTransactionIdListDataFactory::~WalletTransactionIdListDataFactory() {

}

IBlockObject* WalletTransactionIdListDataFactory::makeDataFromBinary(ByteBuffer *in) {
	return WalletTransactionIdListData::fromBinary(in);
}

void WalletTransactionIdListDataFactory::registerData(const AbstractBtreeKey *key, const IBlockObject *data,
		DataNode *dataNode, BtreeStorage *store) const {
	uint64_t dataFpos = dataNode->getDataFpos();
	if(dataFpos != 0){
		IBlockObject* obj = store->loadData(dataFpos); __STP(obj);

		WalletTransactionIdListData* baseValue = dynamic_cast<WalletTransactionIdListData*>(obj);
		const WalletTransactionIdListData* newValue = dynamic_cast<const WalletTransactionIdListData*>(data);

		baseValue->join(newValue);
		dataFpos = store->storeData(baseValue, dataFpos);
		dataNode->setDataFpos(dataFpos);

		return;
	}

	dataFpos = store->storeData(data);
	dataNode->setDataFpos(dataFpos);
}

bool WalletTransactionIdListDataFactory::beforeRemove(DataNode *dataNode, BtreeStorage *store, const AbstractBtreeKey *key) const {
	uint64_t dataFpos = dataNode->getDataFpos();

	IBlockObject* obj = store->loadData(dataFpos); __STP(obj);
	WalletTransactionIdListData* baseValue = dynamic_cast<WalletTransactionIdListData*>(obj);

	const SystemTimestampKey* fkey = dynamic_cast<const SystemTimestampKey*>(key);

	const TransactionId* value = dynamic_cast<const TransactionId*>(fkey->getRemoveKey());
	baseValue->remove(value);

	if(baseValue->isEmpty()){
		return true; // remove
	}

	dataFpos = store->storeData(baseValue, dataFpos);
	dataNode->setDataFpos(dataFpos);

	return false; // do not remove
}

AbstractBtreeDataFactory* WalletTransactionIdListDataFactory::copy() const noexcept {
	return new WalletTransactionIdListDataFactory();
}

} /* namespace codablecash */
