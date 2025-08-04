/*
 * ManagedUtxoCache.cpp
 *
 *  Created on: Jul 17, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_data_management/ManagedUtxoCache.h"
#include "bc_wallet_net_data_management/ManagedUtxoCacheRecord.h"

#include "bc_trx/UtxoId.h"

#include "base/StackRelease.h"
namespace codablecash {

ManagedUtxoCache::ManagedUtxoCache() {
	this->list = new ArrayList<ManagedUtxoCacheRecord>();
	this->map = new HashMap<UtxoId, ManagedUtxoCacheRecord>();
}

ManagedUtxoCache::~ManagedUtxoCache() {
	delete this->map;

	this->list->deleteElements();
	delete this->list;
}

void ManagedUtxoCache::reset() noexcept {
	this->map->clear();

	this->list->deleteElements();
	this->list->reset();
}


/**
 *
 * @param utxo
 * @param trxId
 * @param storeType ManagedUtxoCacheRecord::FINALIZED, UNFINALIZED, MEMPOOL
 */
void ManagedUtxoCache::addUtxo(const AbstractUtxo *utxo, const TransactionId* trxId, int storeType) {
	ManagedUtxoCacheRecord* record = new ManagedUtxoCacheRecord();
	record->setUtxo(utxo);
	record->setType(storeType);
	record->setTransactionId(trxId);

	this->list->addElement(record);

	const UtxoId* utxoId = record->getUtxoId();
	this->map->put(utxoId, record);
}

const ManagedUtxoCacheRecord* ManagedUtxoCache::getManagedUtxoCacheRecord(const UtxoId *utxoId) const noexcept {
	ManagedUtxoCacheRecord* record = this->map->get(utxoId);
	return record;
}

void ManagedUtxoCache::removeUtxo(const UtxoId *utxoId) noexcept {
	ManagedUtxoCacheRecord* record = this->map->get(utxoId); __STP(record);

	if(record != nullptr){
		this->map->remove(utxoId);

		int index = this->list->indexOfPtr(record);
		this->list->remove(index);
	}
}

bool ManagedUtxoCache::hasUtxo(const UtxoId *utxoId) const noexcept {
	return this->getManagedUtxoCacheRecord(utxoId) != nullptr;
}

void ManagedUtxoCache::importOtherManagedUtxoCache(const ManagedUtxoCache *other) {
	int maxLoop = other->list->size();
	for(int i = 0; i != maxLoop; ++i){
		const ManagedUtxoCacheRecord* record = other->list->get(i);

		const AbstractUtxo* utxo = record->getUtxo();
		uint8_t storeType = record->getType();
		const TransactionId* trxId = record->getTransactionId();

		addUtxo(utxo, trxId, storeType);
	}
}

} /* namespace codablecash */
