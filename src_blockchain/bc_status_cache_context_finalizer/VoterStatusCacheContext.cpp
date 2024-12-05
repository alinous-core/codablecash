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

#include "bc/CodablecashSystemParam.h"

#include "bc_status_cache_lockin/VotedLockinOperation.h"
#include "bc_status_cache_lockin/ILockinManager.h"
#include "bc_status_cache_lockin/MissvoteLockinOperation.h"

namespace codablecash {

VoterStatusCacheContext::VoterStatusCacheContext(const File* baseDir) {
	this->repo = new VotingBlockStatusRepository(baseDir);
}

VoterStatusCacheContext::~VoterStatusCacheContext() {
	this->repo->close();
	delete this->repo;
}

void VoterStatusCacheContext::init() {
	this->repo->init();
	this->repo->open();
}

void VoterStatusCacheContext::initBlank() {
	this->repo->init();
}

void VoterStatusCacheContext::open() {
	this->repo->open();
}

void VoterStatusCacheContext::close() {
	this->repo->close();
}

void VoterStatusCacheContext::importRepo(const VoterStatusCacheContext *other) {
	BtreeScanner* scanner = other->repo->getBtreeScanner(); __STP(scanner);
	while(scanner->hasNext()){
		const IBlockObject* obj = scanner->next();
		const VotingBlockStatus* status = dynamic_cast<const VotingBlockStatus*>(obj);

		this->repo->storeVotingBlockStatus(status);
	}
}

void VoterStatusCacheContext::storeVotingBlockStatus(const VotingBlockStatus *status) {
	this->repo->storeVotingBlockStatus(status);
}

VotingBlockStatus* VoterStatusCacheContext::getVotingBlockStatus(const BlockHeaderId *blockHeaderId) {
	return this->repo->getVotingBlockStatus(blockHeaderId);
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

void VoterStatusCacheContext::clean(uint64_t height) {
	this->repo->clean(height);
}

} /* namespace codablecash */
