/*
 * VoterStatusCacheContext.cpp
 *
 *  Created on: 2023/04/25
 *      Author: iizuka
 */

#include "bc_status_cache_context_finalizer/VoterStatusCacheContext.h"
#include "bc_status_cache_context_finalizer/VotingBlockStatusRepository.h"
#include "bc_status_cache_context_finalizer/VotingBlockStatus.h"
#include "bc_status_cache_context_finalizer/VoteCandidate.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_finalizer/VoterEntry.h"

#include "base/StackRelease.h"

#include "btree/BtreeScanner.h"

#include "bc_status_cache/ZoneStatusCache.h"
#include "bc_status_cache/BlockchainStatusCache.h"

#include "bc_status_cache_data/FinalizedVoterRepository.h"
#include "bc_status_cache_data/FinalizedDataCache.h"

#include "bc_trx/UtxoId.h"
namespace codablecash {

VoterStatusCacheContext::VoterStatusCacheContext(const File* baseDir) {
	this->votersMap = new HashMap<NodeIdentifier, VoterEntry>();
	this->repo = new VotingBlockStatusRepository(baseDir);
}

VoterStatusCacheContext::~VoterStatusCacheContext() {
	Iterator<NodeIdentifier>* it = this->votersMap->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const NodeIdentifier* key = it->next();
		VoterEntry* entry = this->votersMap->get(key);

		delete entry;
	}

	delete this->votersMap;

	this->repo->close();
	delete this->repo;
}

void VoterStatusCacheContext::init() {
	this->repo->init();
	this->repo->open();
}

void VoterStatusCacheContext::addNewVoter(VoterEntry *entry) {
	const NodeIdentifier* nodeId = entry->getNodeIdentifier();

#ifdef __DEBUG__
	VoterEntry *e = this->votersMap->get(nodeId);
	assert(e == nullptr);
#endif

	this->votersMap->put(nodeId, entry);
}

void VoterStatusCacheContext::listEntries(ArrayList<VoterEntry, VoterEntry::VoteCompare> *list) const noexcept {
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

void VoterStatusCacheContext::importMap(const VoterStatusCacheContext *other) {
	Iterator<NodeIdentifier>* it = other->votersMap->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const NodeIdentifier* key = it->next();
		VoterEntry* entry = other->votersMap->get(key);

		addNewVoter(new VoterEntry(*entry));
	}
}

void VoterStatusCacheContext::importRepo(const VoterStatusCacheContext *other) {
	BtreeScanner* scanner = other->repo->getBtreeScanner(); __STP(scanner);
	while(scanner->hasNext()){
		const IBlockObject* obj = scanner->next();
		const VotingBlockStatus* status = dynamic_cast<const VotingBlockStatus*>(obj);

		this->repo->storeVotingBlockStatus(status);
	}
}

const VoterEntry* VoterStatusCacheContext::getVoterEntry(const NodeIdentifier *nodeId) const noexcept {
	return this->votersMap->get(nodeId);
}

void VoterStatusCacheContext::loadFinalyzedVoters(uint16_t zone, BlockchainStatusCache* statusCache) {
	ZoneStatusCache* cache = statusCache->getZoneStatusCache(zone);
	FinalizedDataCache* fcache = cache->getFinalizedDataCache();
	FinalizedVoterRepository* voterRepo = fcache->getFinalizedVoterRepository();

	voterRepo->exportAllToMap(this->votersMap);
}

void VoterStatusCacheContext::loadLastCache(const VoterStatusCacheContext *lastVoterCache) {
	const HashMap<NodeIdentifier, VoterEntry>* m = lastVoterCache->getVotersMap();

	Iterator<NodeIdentifier>* it = m->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const NodeIdentifier* key = it->next();
		VoterEntry* entry = m->get(key);

		this->votersMap->put(key, new VoterEntry(*entry));
	}
}

void VoterStatusCacheContext::storeVotingBlockStatus(const VotingBlockStatus *status) {
	this->repo->storeVotingBlockStatus(status);
}

void VoterStatusCacheContext::removeVotedTicket(const VotingBlockStatus *status) {
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

VotingBlockStatus* VoterStatusCacheContext::getVotingBlockStatus(const BlockHeaderId *blockHeaderId) {
	return this->repo->getVotingBlockStatus(blockHeaderId);
}

void VoterStatusCacheContext::handleVotedStatus(VotingBlockStatus *status, int missingLimit, int extendCount) {
	const ArrayList<VoteCandidate>* list = status->getCandidatesList();

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		const VoteCandidate* candidate = list->get(i);

		const NodeIdentifier* nodeId = candidate->getNodeIdentifier();
		VoterEntry* entry = this->votersMap->get(nodeId);

		bool voted = candidate->isVoted();
		if(voted){
			entry->handleVoted(extendCount);
		}else{
			entry->handleMissed(missingLimit);
		}
	}
}

void VoterStatusCacheContext::handleVote(VotingBlockStatus *status, const UtxoId *utxoId) {
	const ArrayList<VoteCandidate>* list = status->getCandidatesList();

#ifdef __DEBUG__
	int count = 0;
#endif

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		VoteCandidate* candidate = list->get(i);

		const UtxoId* candidateUtxoId = candidate->getUtxoId();
		if(candidateUtxoId->equals(utxoId)){
#ifdef __DEBUG__
			count++;
			assert(candidate->isVoted() == false);
#endif
			candidate->setVoted(true);
			break;
		}
	}

#ifdef __DEBUG__
	assert(count == 1);
#endif
}

} /* namespace codablecash */
