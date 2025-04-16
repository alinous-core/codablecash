/*
 * HeadBlockDetectorCacheElement.cpp
 *
 *  Created on: 2023/05/25
 *      Author: iizuka
 */

#include "bc_status_cache/HeadBlockDetectorCacheElement.h"
#include "bc_status_cache/BlockHeadElement.h"

#include "bc_trx/TransactionId.h"

#include "bc_memorypool/MemPoolTransaction.h"

namespace codablecash {

HeadBlockDetectorCacheElement::HeadBlockDetectorCacheElement() {
	this->mevHealthScore = 0;
	this->voterScore = 0;
	this->votingScore = 0;
}

HeadBlockDetectorCacheElement::~HeadBlockDetectorCacheElement() {
	this->list.deleteElements();
}

void HeadBlockDetectorCacheElement::consume(const TransactionId *trxId) noexcept {
	TransactionId* id = dynamic_cast<TransactionId*>(trxId->copyData());
	this->list.addElement(id);
}

void HeadBlockDetectorCacheElement::importBlockHeadElement(const BlockHeadElement *element) {
	this->mevHealthScore = element->getMevHealthScore();
	this->voterScore = element->getVotedScore();
	this->votingScore = element->getVotingSocre();
}

void HeadBlockDetectorCacheElement::export2BlockHeadElement(BlockHeadElement *element, MemPoolTransaction* memTransaction, bool headerOnly) const noexcept {
	if(!headerOnly){
		int maxLoop = this->list.size();
		for(int i = 0; i != maxLoop; ++i){
			TransactionId* trxId = this->list.get(i);
			memTransaction->consume(trxId);
		}
	}

	element->setVotedScore(this->voterScore);
	element->setVotingScore(this->votingScore);
	element->setMevHealthScore(this->mevHealthScore);
}

} /* namespace codablecash */
