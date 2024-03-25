/*
 * BlockHeaderHeightIndexDataFactory.cpp
 *
 *  Created on: 2023/02/16
 *      Author: iizuka
 */

#include "bc_blockstore_header/BlockHeaderHeightIndexDataFactory.h"
#include "bc_blockstore_header/BlockHeaderList.h"
#include "bc_blockstore_header/BlockHeaderHeightIndexKey.h"

#include "btree/DataNode.h"
#include "btree/BtreeStorage.h"

#include "base/StackRelease.h"

#include "filestore_block/IBlockObject.h"


namespace codablecash {

BlockHeaderHeightIndexDataFactory::BlockHeaderHeightIndexDataFactory() {

}

BlockHeaderHeightIndexDataFactory::~BlockHeaderHeightIndexDataFactory() {

}

IBlockObject* BlockHeaderHeightIndexDataFactory::makeDataFromBinary(ByteBuffer *in) {
	return BlockHeaderList::fromBinary(in);
}

void BlockHeaderHeightIndexDataFactory::registerData(
		const AbstractBtreeKey *key, const IBlockObject *data,
		DataNode *dataNode, BtreeStorage *store) const {
	uint64_t dataFpos = dataNode->getDataFpos();
	if(dataFpos != 0){
		IBlockObject* obj = store->loadData(dataFpos); __STP(obj);

		BlockHeaderList* baseValue = dynamic_cast<BlockHeaderList*>(obj);
		const BlockHeaderList* newValue = dynamic_cast<const BlockHeaderList*>(data);

		baseValue->join(newValue);
		dataFpos = store->storeData(baseValue, dataFpos);
		dataNode->setDataFpos(dataFpos);

		return;
	}

	dataFpos = store->storeData(data);
	dataNode->setDataFpos(dataFpos);
}

bool BlockHeaderHeightIndexDataFactory::beforeRemove(DataNode *dataNode,
		BtreeStorage *store, const AbstractBtreeKey *key) const {
	uint64_t dataFpos = dataNode->getDataFpos();

	IBlockObject* obj = store->loadData(dataFpos); __STP(obj);
	BlockHeaderList* baseValue = dynamic_cast<BlockHeaderList*>(obj);

	const BlockHeaderHeightIndexKey* fkey = dynamic_cast<const BlockHeaderHeightIndexKey*>(key);

	uint64_t value = fkey->getFpos();
	baseValue->remove(value);

	if(baseValue->isEmpty()){
		return true; // remove
	}

	dataFpos = store->storeData(baseValue, dataFpos);
	dataNode->setDataFpos(dataFpos);

	return false; // do not remove
}

AbstractBtreeDataFactory* BlockHeaderHeightIndexDataFactory::copy() const noexcept {
	return new BlockHeaderHeightIndexDataFactory();
}

} /* namespace codablecash */
