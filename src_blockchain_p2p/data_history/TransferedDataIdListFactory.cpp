/*
 * TransferedDataIdListFactory.cpp
 *
 *  Created on: 2023/09/19
 *      Author: iizuka
 */

#include "data_history/TransferedDataIdListFactory.h"
#include "data_history/TransferedDataIdList.h"

#include "btree/DataNode.h"
#include "btree/BtreeStorage.h"
#include "btree/AbstractBtreeKey.h"

#include "filestore_block/IBlockObject.h"

#include "base/StackRelease.h"

#include "data_history/TimeIndexKey.h"
namespace codablecash {

TransferedDataIdListFactory::TransferedDataIdListFactory() {

}

TransferedDataIdListFactory::~TransferedDataIdListFactory() {

}

IBlockObject* TransferedDataIdListFactory::makeDataFromBinary(ByteBuffer *in) {
	return TransferedDataIdList::fromBinary(in);
}

void TransferedDataIdListFactory::registerData(const AbstractBtreeKey *key,const IBlockObject *data, DataNode *dataNode, BtreeStorage *store) const {
	uint64_t dataFpos = dataNode->getDataFpos();
	if(dataFpos != 0){
		IBlockObject* obj = store->loadData(dataFpos); __STP(obj);

		TransferedDataIdList* baseValue = dynamic_cast<TransferedDataIdList*>(obj);
		const TransferedDataIdList* newValue = dynamic_cast<const TransferedDataIdList*>(data);

		baseValue->join(newValue);
		dataFpos = store->storeData(baseValue, dataFpos);

		dataNode->setDataFpos(dataFpos);

		return;
	}

	dataFpos = store->storeData(data);
	dataNode->setDataFpos(dataFpos);
}

bool TransferedDataIdListFactory::beforeRemove(DataNode *dataNode, BtreeStorage *store, const AbstractBtreeKey *key) const {
	uint64_t dataFpos = dataNode->getDataFpos();

	IBlockObject* obj = store->loadData(dataFpos); __STP(obj);
	TransferedDataIdList* baseValue = dynamic_cast<TransferedDataIdList*>(obj);

	const TimeIndexKey* fkey = dynamic_cast<const TimeIndexKey*>(key);

	const TransferedDataId* value = fkey->getDataId();
	//baseValue
	baseValue->remove(value);

	if(baseValue->isEmpty()){
		return true; // remove
	}

	dataFpos = store->storeData(baseValue, dataFpos);
	dataNode->setDataFpos(dataFpos);

	return false; // do not remove
}

AbstractBtreeDataFactory* TransferedDataIdListFactory::copy() const noexcept {
	return new TransferedDataIdListFactory();
}

} /* namespace codablecash */
