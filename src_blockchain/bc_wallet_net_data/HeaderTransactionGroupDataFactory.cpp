/*
 * HeaderTransactionGroupDataFactory.cpp
 *
 *  Created on: Jul 4, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_data/HeaderTransactionGroupDataFactory.h"
#include "bc_wallet_net_data/HeaderTransactionGroup.h"

#include "btree/DataNode.h"
#include "btree/BtreeStorage.h"

#include "base/StackRelease.h"

#include "bc_blockstore_header/BlockHeaderIdKey.h"


namespace codablecash {

HeaderTransactionGroupDataFactory::HeaderTransactionGroupDataFactory() {

}

HeaderTransactionGroupDataFactory::~HeaderTransactionGroupDataFactory() {

}

IBlockObject* HeaderTransactionGroupDataFactory::makeDataFromBinary(ByteBuffer *in) {
	return HeaderTransactionGroup::createFromBinary(in);

}

void HeaderTransactionGroupDataFactory::registerData(const AbstractBtreeKey *key, const IBlockObject *data, DataNode *dataNode, BtreeStorage *store) const {
	uint64_t dataFpos = dataNode->getDataFpos();

	if(dataFpos != 0){
		IBlockObject* obj = store->loadData(dataFpos); __STP(obj);

		HeaderTransactionGroup* baseValue = dynamic_cast<HeaderTransactionGroup*>(obj);
		const HeaderTransactionGroup* newValue = dynamic_cast<const HeaderTransactionGroup*>(data);

		baseValue->join(newValue);
		dataFpos = store->storeData(baseValue, dataFpos);
		dataNode->setDataFpos(dataFpos);

		return;
	}

	dataFpos = store->storeData(data);
	dataNode->setDataFpos(dataFpos);
}

bool HeaderTransactionGroupDataFactory::beforeRemove(DataNode *dataNode, BtreeStorage *store, const AbstractBtreeKey *key) const {
	return true; // remove
}

AbstractBtreeDataFactory* HeaderTransactionGroupDataFactory::copy() const noexcept {
	return new HeaderTransactionGroupDataFactory();
}

} /* namespace codablecash */
