/*
 * VoterStatusMappedCacheContext.cpp
 *
 *  Created on: Nov 4, 2024
 *      Author: iizuka
 */

#include "bc_status_cache_context_finalizer/VoterStatusMappedCacheContext.h"

#include "base/StackRelease.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_status_cache/ZoneStatusCache.h"
#include "bc_status_cache/BlockchainStatusCache.h"

#include "bc_status_cache_data/FinalizedDataCache.h"
#include "bc_status_cache_data/FinalizedVoterRepository.h"

#include "bc_status_cache_context_finalizer/VoteCandidate.h"
#include "bc_status_cache_context_finalizer/VotingBlockStatus.h"

#include "base/ArrayList.h"

#include "bc/CodablecashSystemParam.h"

#include "bc_status_cache_lockin/ILockinManager.h"
#include "bc_status_cache_lockin/VotedLockinOperation.h"
#include "bc_status_cache_lockin/MissvoteLockinOperation.h"


namespace codablecash {

VoterStatusMappedCacheContext::VoterStatusMappedCacheContext(const File* baseDir)
		: VoterStatusCacheContext(baseDir) {
	this->votersMap = new HashMap<NodeIdentifier, VoterEntry>();
}

VoterStatusMappedCacheContext::~VoterStatusMappedCacheContext() {
	Iterator<NodeIdentifier>* it = this->votersMap->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const NodeIdentifier* key = it->next();
		VoterEntry* entry = this->votersMap->get(key);

		delete entry;
	}

	delete this->votersMap;
}

void VoterStatusMappedCacheContext::addNewVoter(VoterEntry *entry) {
	const NodeIdentifier* nodeId = entry->getNodeIdentifier();

#ifdef __DEBUG__
	VoterEntry *e = this->votersMap->get(nodeId);
	assert(e == nullptr);
#endif

	this->votersMap->put(nodeId, entry);
}

void VoterStatusMappedCacheContext::listEntries(ArrayList<VoterEntry, VoterEntry::VoteCompare> *list) const noexcept {
	list->sort();

	Iterator<NodeIdentifier>* it = this->votersMap->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const NodeIdentifier* key = it->next();
		VoterEntry* entry = this->votersMap->get(key);

		if(list->search(entry) == nullptr){
			list->addElement(new VoterEntry(*entry));
		}
	}
}

void VoterStatusMappedCacheContext::importMap(const VoterStatusMappedCacheContext *other) {
	Iterator<NodeIdentifier>* it = other->votersMap->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const NodeIdentifier* key = it->next();
		VoterEntry* entry = other->votersMap->get(key);

		addNewVoter(new VoterEntry(*entry));
	}
}

void VoterStatusMappedCacheContext::loadLastCache(const VoterStatusMappedCacheContext *lastVoterCache) {
	const HashMap<NodeIdentifier, VoterEntry>* m = lastVoterCache->getVotersMap();

	Iterator<NodeIdentifier>* it = m->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const NodeIdentifier* key = it->next();
		VoterEntry* entry = m->get(key);

		this->votersMap->put(key, new VoterEntry(*entry));
	}
}


void VoterStatusMappedCacheContext::loadFinalyzedVoters(uint16_t zone, BlockchainStatusCache *statusCache) {
	ZoneStatusCache* cache = statusCache->getZoneStatusCache(zone);
	FinalizedDataCache* fcache = cache->getFinalizedDataCache();
	FinalizedVoterRepository* voterRepo = fcache->getFinalizedVoterRepository();

	voterRepo->exportAllToMap(this->votersMap);
}

const VoterEntry* VoterStatusMappedCacheContext::getVoterEntry(const NodeIdentifier *nodeId) const noexcept {
	return this->votersMap->get(nodeId);
}

void VoterStatusMappedCacheContext::removeVotedTicket(const VotingBlockStatus *status) {
	const ArrayList<VoteCandidate>* list = status->getCandidatesList();

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		VoteCandidate* candidate = list->get(i);

		const NodeIdentifier *nodeId = candidate->getNodeIdentifier();
		const VoterEntry* voteEntry = getVoterEntry(nodeId);
		assert(voteEntry != nullptr);

		const UtxoId* utxoId = candidate->getUtxoId();
		bool deleted = const_cast<VoterEntry*>(voteEntry)->removeTicket(utxoId);

		assert(deleted == true);
	}
}

void VoterStatusMappedCacheContext::handleVotedStatus(uint64_t height,
		VotingBlockStatus *status, int missingLimit, int extendCount,
		ILockinManager *lockinManager, const CodablecashSystemParam *params) {
	const ArrayList<VoteCandidate>* list = status->getCandidatesList();

	uint64_t voteLockinIntervalHeight = params->getVoteLockinIntervalHeight();
	uint64_t finalizingHeight = lockinManager->getFinalizingHeight();
	uint64_t lockinHeight = finalizingHeight + voteLockinIntervalHeight;

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		const VoteCandidate* candidate = list->get(i);

		const NodeIdentifier* nodeId = candidate->getNodeIdentifier();
		// VoterEntry* entry = this->votersMap->get(nodeId);

		const UtxoId* utxoId = candidate->getUtxoId();

		bool voted = candidate->isVoted();
		if(voted){
			VotedLockinOperation operation;
			operation.setNodeId(nodeId);
			operation.setUtxoId(utxoId);

			lockinManager->addOperation(lockinHeight, &operation);
		}else{
			MissvoteLockinOperation operation;
			operation.setNodeId(nodeId);
			operation.setUtxoId(utxoId);

			lockinManager->addOperation(lockinHeight, &operation);
		}
	}
}

} /* namespace codablecash */
