/*
 * VotePart.cpp
 *
 *  Created on: 2023/05/06
 *      Author: iizuka
 */

#include "bc_block_vote/VotePart.h"
#include "bc_block_vote/VotedHeaderIdGroup.h"

#include "base/StackRelease.h"

#include "bc_block/BlockHeaderId.h"

#include "bc_finalizer_trx/VoteBlockTransaction.h"

namespace codablecash {

VotePart::VotePart() {
	this->map = new HashMap<BlockHeaderId, VotedHeaderIdGroup>();
}

VotePart::~VotePart() {
	Iterator<BlockHeaderId>* it = this->map->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const BlockHeaderId* headerId = it->next();
		VotedHeaderIdGroup* group = this->map->get(headerId);

		delete group;
	}

	delete this->map;
}

void VotePart::addVote(const VoteBlockTransaction *trx) noexcept {
	const BlockHeaderId* headerId = trx->getVotedHeaderId();

	VotedHeaderIdGroup* group = this->map->get(headerId);
	if(group == nullptr){
		group = new VotedHeaderIdGroup(headerId);
		this->map->put(headerId, group);
	}

	const TransactionId* trxId = trx->getTransactionId();
	group->add(trxId);
}

int VotePart::binarySize() const {
	int total = sizeof(uint8_t);

	Iterator<BlockHeaderId>* it = this->map->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const BlockHeaderId* headerId = it->next();
		VotedHeaderIdGroup* group = this->map->get(headerId);

		total += group->binarySize();
	}

	return total;
}

void VotePart::toBinary(ByteBuffer *out) const {
	int size = this->map->size();
	out->put(size);

	Iterator<BlockHeaderId>* it = this->map->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const BlockHeaderId* headerId = it->next();
		VotedHeaderIdGroup* group = this->map->get(headerId);

		group->toBinary(out);
	}
}

VotePart* VotePart::createFromBinary(ByteBuffer *in) {
	VotePart* part = new VotePart();

	int maxLoop = in->get();

	for(int i = 0; i != maxLoop; ++i){
		VotedHeaderIdGroup* group = VotedHeaderIdGroup::createFromBinary(in);
		const BlockHeaderId* headerId = group->getBlockHeaderId();

		part->map->put(headerId, group);
	}

	return part;
}

const VotedHeaderIdGroup* VotePart::getMaxVotedGroup() const noexcept {
	VotedHeaderIdGroup* retGroup = nullptr;
	int lastsize = 0;

	Iterator<BlockHeaderId>* it = this->map->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const BlockHeaderId* headerId = it->next();
		VotedHeaderIdGroup* group = this->map->get(headerId);

		if(lastsize < group->size()){
			retGroup = group;
			lastsize = group->size();
		}
	}

	return retGroup;
}

const VotedHeaderIdGroup* VotePart::getVotedGroup(const BlockHeaderId *votedHeaderId) const noexcept {
	return this->map->get(votedHeaderId);
}

void VotePart::buildVoteTransactionIdCertificate(const Block *block, IVoteTransactionIdCertificateBuilder *callback) const {
	Iterator<BlockHeaderId>* it = this->map->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const BlockHeaderId* headerId = it->next();
		VotedHeaderIdGroup* group = this->map->get(headerId);

		group->buildVoteTransactionIdCertificate(block, callback);
	}
}

void VotePart::visitVoteTransactionIdCertificate(IVoteTransactionIdCertificatevisitor *visitor) const {
	Iterator<BlockHeaderId>* it = this->map->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const BlockHeaderId* headerId = it->next();
		VotedHeaderIdGroup* group = this->map->get(headerId);

		group->visitVoteTransactionIdCertificate(visitor);
	}
}

} /* namespace codablecash */
