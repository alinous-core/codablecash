/*
 * SyncHeaderDataFactory.cpp
 *
 *  Created on: 2023/11/06
 *      Author: iizuka
 */

#include "bc_network_instance_sync_repo/SyncHeaderDataFactory.h"
#include "bc_network_instance_sync_repo/HeightHeaderSyncDataList.h"

#include "btree/DataNode.h"
#include "btree/BtreeStorage.h"

#include "base/StackRelease.h"

namespace codablecash {

SyncHeaderDataFactory::SyncHeaderDataFactory() {

}

SyncHeaderDataFactory::~SyncHeaderDataFactory() {

}

IBlockObject* SyncHeaderDataFactory::makeDataFromBinary(ByteBuffer *in) {
	return HeightHeaderSyncDataList::createFromBinary(in);
}

void SyncHeaderDataFactory::registerData(const AbstractBtreeKey *key,
		const IBlockObject *data, DataNode *dataNode, BtreeStorage *store) const {
	uint64_t dataFpos = dataNode->getDataFpos();
	if(dataFpos != 0){
		IBlockObject* obj = store->loadData(dataFpos); __STP(obj);

		HeightHeaderSyncDataList* baseValue = dynamic_cast<HeightHeaderSyncDataList*>(obj);
		const HeightHeaderSyncDataList* newValue = dynamic_cast<const HeightHeaderSyncDataList*>(data);

		baseValue->join(newValue);
		dataFpos = store->storeData(baseValue, dataFpos);
		dataNode->setDataFpos(dataFpos);

		return;
	}

	dataFpos = store->storeData(data);
	dataNode->setDataFpos(dataFpos);
}

AbstractBtreeDataFactory* SyncHeaderDataFactory::copy() const noexcept {
	return new SyncHeaderDataFactory();
}

} /* namespace codablecash */
