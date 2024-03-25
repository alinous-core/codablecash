/*
 * AddressDescriptorUtxoDataFactory.cpp
 *
 *  Created on: 2023/02/23
 *      Author: iizuka
 */

#include "bc_base_utxo_index/AddressDescriptorUtxoDataFactory.h"
#include "bc_base_utxo_index/AddressDescriptorUtxoData.h"
#include "bc_base_utxo_index/AddressDescriptorKey.h"

#include "btree/DataNode.h"

#include "btree/BtreeStorage.h"

#include "base/StackRelease.h"

namespace codablecash {

AddressDescriptorUtxoDataFactory::AddressDescriptorUtxoDataFactory() {

}

AddressDescriptorUtxoDataFactory::~AddressDescriptorUtxoDataFactory() {

}

IBlockObject* AddressDescriptorUtxoDataFactory::makeDataFromBinary(ByteBuffer *in) {
	return AddressDescriptorUtxoData::fromBinary(in);
}

void AddressDescriptorUtxoDataFactory::registerData(const AbstractBtreeKey *key,
		const IBlockObject *data, DataNode *dataNode, BtreeStorage *store) const {
	uint64_t dataFpos = dataNode->getDataFpos();
	if(dataFpos != 0){
		IBlockObject* obj = store->loadData(dataFpos); __STP(obj);

		AddressDescriptorUtxoData* baseValue = dynamic_cast<AddressDescriptorUtxoData*>(obj);
		const AddressDescriptorUtxoData* newValue = dynamic_cast<const AddressDescriptorUtxoData*>(data);

		baseValue->join(newValue);
		dataFpos = store->storeData(baseValue, dataFpos);

		dataNode->setDataFpos(dataFpos);

		return;
	}

	dataFpos = store->storeData(data);
	dataNode->setDataFpos(dataFpos);
}

bool AddressDescriptorUtxoDataFactory::beforeRemove(DataNode *dataNode,
		BtreeStorage *store, const AbstractBtreeKey *key) const {
	uint64_t dataFpos = dataNode->getDataFpos();

	IBlockObject* obj = store->loadData(dataFpos); __STP(obj);
	AddressDescriptorUtxoData* baseValue = dynamic_cast<AddressDescriptorUtxoData*>(obj);

	const AddressDescriptorKey* fkey = dynamic_cast<const AddressDescriptorKey*>(key);

	const AbstractUtxo* value = fkey->getUtxo();
	//baseValue
	baseValue->remove(value);

	if(baseValue->isEmpty()){
		return true; // remove
	}

	dataFpos = store->storeData(baseValue, dataFpos);
	dataNode->setDataFpos(dataFpos);

	return false; // do not remove
}

AbstractBtreeDataFactory* AddressDescriptorUtxoDataFactory::copy() const noexcept {
	return new AddressDescriptorUtxoDataFactory();
}

} /* namespace codablecash */
