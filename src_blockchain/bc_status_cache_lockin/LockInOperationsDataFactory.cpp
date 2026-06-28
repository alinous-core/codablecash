/*
 * LockInOperationDataFactory.cpp
 *
 *  Created on: Oct 18, 2024
 *      Author: iizuka
 */

#include "btree/DataNode.h"
#include "btree/BtreeStorage.h"

#include "base/StackRelease.h"
#include "bc_status_cache_lockin/LockInOperationsData.h"
#include "bc_status_cache_lockin/LockInOperationsDataFactory.h"


namespace codablecash {

LockInOperationsDataFactory::LockInOperationsDataFactory() {

}

LockInOperationsDataFactory::~LockInOperationsDataFactory() {
}

IBlockObject* LockInOperationsDataFactory::makeDataFromBinary(ByteBuffer *in) {
	return LockInOperationsData::fromBinary(in);
}

void LockInOperationsDataFactory::registerData(const AbstractBtreeKey *key, const IBlockObject *data, DataNode *dataNode, BtreeStorage *store) const {
	uint64_t dataFpos = dataNode->getDataFpos();
	if(dataFpos != 0){
		IBlockObject* obj = store->loadData(dataFpos); __STP(obj);

		LockInOperationsData* baseValue = dynamic_cast<LockInOperationsData*>(obj);
		const LockInOperationsData* newValue = dynamic_cast<const LockInOperationsData*>(data);

		baseValue->join(newValue);
		dataFpos = store->storeData(baseValue, dataFpos);

		dataNode->setDataFpos(dataFpos);

		return;
	}

	dataFpos = store->storeData(data);
	dataNode->setDataFpos(dataFpos);
}

bool LockInOperationsDataFactory::beforeRemove(DataNode *dataNode, BtreeStorage *store, const AbstractBtreeKey *key) const {
	return true; // remove
}

AbstractBtreeDataFactory* LockInOperationsDataFactory::copy() const noexcept {
	return new LockInOperationsDataFactory();
}

} /* namespace codablecash */
