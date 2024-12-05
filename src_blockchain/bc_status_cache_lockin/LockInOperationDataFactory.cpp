/*
 * LockInOperationDataFactory.cpp
 *
 *  Created on: Oct 18, 2024
 *      Author: iizuka
 */

#include "bc_status_cache_lockin/LockInOperationDataFactory.h"
#include "bc_status_cache_lockin/LockInOperationData.h"

#include "btree/DataNode.h"
#include "btree/BtreeStorage.h"

#include "base/StackRelease.h"


namespace codablecash {

LockInOperationDataFactory::LockInOperationDataFactory() {

}

LockInOperationDataFactory::~LockInOperationDataFactory() {
}

IBlockObject* LockInOperationDataFactory::makeDataFromBinary(ByteBuffer *in) {
	return LockInOperationData::fromBinary(in);
}

void LockInOperationDataFactory::registerData(const AbstractBtreeKey *key, const IBlockObject *data, DataNode *dataNode, BtreeStorage *store) const {
	uint64_t dataFpos = dataNode->getDataFpos();
	if(dataFpos != 0){
		IBlockObject* obj = store->loadData(dataFpos); __STP(obj);

		LockInOperationData* baseValue = dynamic_cast<LockInOperationData*>(obj);
		const LockInOperationData* newValue = dynamic_cast<const LockInOperationData*>(data);

		baseValue->join(newValue);
		dataFpos = store->storeData(baseValue, dataFpos);

		dataNode->setDataFpos(dataFpos);

		return;
	}

	dataFpos = store->storeData(data);
	dataNode->setDataFpos(dataFpos);
}

bool LockInOperationDataFactory::beforeRemove(DataNode *dataNode, BtreeStorage *store, const AbstractBtreeKey *key) const {
	return true; // remove
}

AbstractBtreeDataFactory* LockInOperationDataFactory::copy() const noexcept {
	return new LockInOperationDataFactory();
}

} /* namespace codablecash */
