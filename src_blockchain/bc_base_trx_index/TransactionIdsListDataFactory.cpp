/*
 * TransactionIdsListDataFactory.cpp
 *
 *  Created on: 2023/02/28
 *      Author: iizuka
 */

#include "bc_base_trx_index/TransactionIdsListDataFactory.h"
#include "bc_base_trx_index/TransactionIdsListData.h"
#include "bc_base_trx_index/BalanceUnitKey.h"

#include "btree/DataNode.h"
#include "btree/BtreeStorage.h"

#include "base/StackRelease.h"


namespace codablecash {

TransactionIdsListDataFactory::TransactionIdsListDataFactory() {

}

TransactionIdsListDataFactory::~TransactionIdsListDataFactory() {

}

IBlockObject* TransactionIdsListDataFactory::makeDataFromBinary(ByteBuffer *in) {
	return TransactionIdsListData::fromBinary(in);
}

void TransactionIdsListDataFactory::registerData(const AbstractBtreeKey *key,
		const IBlockObject *data, DataNode *dataNode, BtreeStorage *store) const {
	uint64_t dataFpos = dataNode->getDataFpos();
	if(dataFpos != 0){
		IBlockObject* obj = store->loadData(dataFpos); __STP(obj);

		TransactionIdsListData* baseValue = dynamic_cast<TransactionIdsListData*>(obj);
		const TransactionIdsListData* newValue = dynamic_cast<const TransactionIdsListData*>(data);

		baseValue->join(newValue);
		dataFpos = store->storeData(baseValue, dataFpos);
		dataNode->setDataFpos(dataFpos);

		return;
	}

	dataFpos = store->storeData(data);
	dataNode->setDataFpos(dataFpos);
}

bool TransactionIdsListDataFactory::beforeRemove(DataNode *dataNode,
		BtreeStorage *store, const AbstractBtreeKey *key) const {
	uint64_t dataFpos = dataNode->getDataFpos();

	IBlockObject* obj = store->loadData(dataFpos); __STP(obj);
	TransactionIdsListData* baseValue = dynamic_cast<TransactionIdsListData*>(obj);

	const BalanceUnitKey* fkey = dynamic_cast<const BalanceUnitKey*>(key);

	const TransactionId* value = fkey->getTrxId();
	//baseValue
	baseValue->remove(value);

	if(baseValue->isEmpty()){
		return true; // remove
	}

	dataFpos = store->storeData(baseValue, dataFpos);
	dataNode->setDataFpos(dataFpos);

	return false; // do not remove
}

AbstractBtreeDataFactory* TransactionIdsListDataFactory::copy() const noexcept {
	return new TransactionIdsListDataFactory();
}

} /* namespace codablecash */
