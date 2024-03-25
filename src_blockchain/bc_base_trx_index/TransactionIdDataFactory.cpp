/*
 * TransactionIdDataFactory.cpp
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#include "bc_base_trx_index/TransactionIdDataFactory.h"
#include "bc_base_trx_index/TransactionIdData.h"

#include "btree/BtreeStorage.h"
#include "btree/DataNode.h"

namespace codablecash {

TransactionIdDataFactory::TransactionIdDataFactory() {

}

TransactionIdDataFactory::~TransactionIdDataFactory() {

}

IBlockObject* TransactionIdDataFactory::makeDataFromBinary(ByteBuffer *in) {
	return TransactionIdData::fromBinary(in);
}

void TransactionIdDataFactory::registerData(const AbstractBtreeKey *key,
		const IBlockObject *data, DataNode *dataNode, BtreeStorage *store) const {
	uint64_t dataFpos = store->storeData(data);
	dataNode->setDataFpos(dataFpos);
}

bool TransactionIdDataFactory::beforeRemove(DataNode *dataNode,
		BtreeStorage *store, const AbstractBtreeKey *key) const {
	return true; // always remove
}

AbstractBtreeDataFactory* TransactionIdDataFactory::copy() const noexcept {
	return new TransactionIdDataFactory();
}

} /* namespace codablecash */
