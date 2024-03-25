/*
 * CachedStatusCacheContext.cpp
 *
 *  Created on: 2023/04/26
 *      Author: iizuka
 */

#include "bc_status_cache_context/CachedStatusCacheContext.h"

#include "base/UnicodeString.h"

#include <cstdio>

#include "base_io/File.h"

#include "base_thread/ConcurrentGate.h"

#include "bc_status_cache/BlockchainStatusCache.h"

#include "bc_status_cache_context/TransactionContextCache.h"
#include "bc_status_cache_context/UtxoCacheContext.h"
#include "bc_status_cache_context/CachedStatusCache.h"
#include "bc_status_cache_context_finalizer/VoterStatusCacheContext.h"

#include "bc_trx/AbstractBlockchainTransaction.h"
#include "bc_trx/AbstractUtxo.h"
#include "bc_trx/AbstractUtxoReference.h"
#include "bc_trx/AbstractBalanceTransaction.h"
#include "bc_trx/AbstractControlTransaction.h"

#include "bc_finalizer_trx/RegisterVotePoolTransaction.h"

#include "base/StackRelease.h"

#include "bc_base_utxo_index/UtxoData.h"

namespace codablecash {

CachedStatusCacheContext::CachedStatusCacheContext(const CachedStatusCache* cache, const CodablecashConfig* config, const File* tmpCacheBaseDir,
		uint16_t zone, ConcurrentGate* rwLock, BlockchainStatusCache* statusCache, CodablecashBlockchain* blockchain)
		: StatusCacheContext(config, tmpCacheBaseDir, zone, rwLock, statusCache, blockchain, CACHED_CONTEXT_CACHE_PREFIX){
	this->cache = cache;

	this->ticketPrice = cache->getTicketPrice();
}

CachedStatusCacheContext::~CachedStatusCacheContext() {
	close();
}

void CachedStatusCacheContext::close() {
	StatusCacheContext::close();

	this->cache = nullptr;
}

ArrayList<VoterEntry, VoterEntry::VoteCompare>* CachedStatusCacheContext::getVoterEntries() const {
	ArrayList<VoterEntry, VoterEntry::VoteCompare>* list = StatusCacheContext::getVoterEntries();

	this->cache->exportVoterEntry(list);

	return list;
}

VotingBlockStatus* CachedStatusCacheContext::getVotingBlockStatus(const BlockHeaderId *blockHeaderId) {
	VotingBlockStatus* status = StatusCacheContext::getVotingBlockStatus(blockHeaderId);

	if(status == nullptr){
		status = this->cache->getVotingBlockStatus(blockHeaderId);
	}

	return status;
}

const VoterEntry* CachedStatusCacheContext::getVoterEntry(	const NodeIdentifier *nodeId) const noexcept {
	const VoterEntry* entry = StatusCacheContext::getVoterEntry(nodeId);
	if(entry != nullptr){
		return entry;
	}

	const VoterEntry* cachedEntry = this->cache->getVoterEntry(nodeId);
	if(cachedEntry != nullptr){
		VoterEntry* newVoter = dynamic_cast<VoterEntry*>(cachedEntry->copyData());
		this->voterCache->addNewVoter(newVoter);
		entry = newVoter;
	}

	return entry;
}

AbstractUtxo* CachedStatusCacheContext::getUtxo(const UtxoId *utxoId) const {
	// active
	{
		UtxoData* data = this->utxoCache->getData(utxoId); __STP(data);
		if(data != nullptr && data->isRemoved()){ // explicitly removes
			return nullptr;
		}

		if(data != nullptr){
			const AbstractUtxo* utxo = data->getUtxo();
			return dynamic_cast<AbstractUtxo*>(utxo->copyData());
		}
	}

	// cache
	{
		UtxoData* data = this->cache->getData(utxoId); __STP(data);
		if(data != nullptr && data->isRemoved()){ // explicitly removes
			return nullptr;
		}

		if(data != nullptr){
			const AbstractUtxo* utxo = data->getUtxo();
			return dynamic_cast<AbstractUtxo*>(utxo->copyData());
		}
	}

	// finalized data
	UtxoData* data = this->statusCache->findUtxo(zone, utxoId); __STP(data);
	return data != nullptr ? dynamic_cast<AbstractUtxo*>(data->getUtxo()->copyData()) : nullptr;
}

void CachedStatusCacheContext::putUtxo(const AbstractUtxo *utxo, const AbstractBlockchainTransaction *trx, const BlockHeader *header) {
	StatusCacheContext::putUtxo(utxo, trx, header);
}

void CachedStatusCacheContext::loadInitialVotersData() {
	// load & update this->voterCache by using this->cache
	const VoterStatusCacheContext* lastVoterCache = this->cache->getVoterStatusCacheContext();
	this->voterCache->loadLastCache(lastVoterCache);
	this->voterCache->importRepo(lastVoterCache);
}

void CachedStatusCacheContext::removeUtxo(const AbstractUtxoReference *utxoref, const AbstractBlockchainTransaction *trx) {
	StatusCacheContext::removeUtxo(utxoref, trx);
}

uint64_t CachedStatusCacheContext::getAnalyzedHeight() const noexcept {
	return this->cache->getHeight();
}

} /* namespace codablecash */
