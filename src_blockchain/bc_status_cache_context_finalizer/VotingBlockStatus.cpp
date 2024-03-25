/*
 * VotingBlockStatus.cpp
 *
 *  Created on: 2023/04/28
 *      Author: iizuka
 */

#include "bc_status_cache_context_finalizer/VotingBlockStatus.h"

#include "bc_block/BlockHeaderId.h"
#include "bc_block/BlockHeader.h"

#include "bc_status_cache_context_finalizer/VoteCandidate.h"

#include "base/StackRelease.h"

#include "bc_finalizer/VoteTicket.h"

#include "bc_base/AddressDescriptor.h"

#include "bc_trx/UtxoId.h"
namespace codablecash {

VotingBlockStatus::VotingBlockStatus(const VotingBlockStatus &inst) {
	this->height = inst.height;
	this->blockHeaderId = new BlockHeaderId(*inst.blockHeaderId);

	this->list = new ArrayList<VoteCandidate>();

	int maxLoop = inst.list->size();
	for(int i = 0; i != maxLoop; ++i){
		VoteCandidate* c = inst.list->get(i);

		addVoteCandidate(dynamic_cast<VoteCandidate*>(c->copyData()));
	}
}

VotingBlockStatus::VotingBlockStatus(uint64_t height, const BlockHeaderId* blockHeaderId) {
	this->height = height;
	this->blockHeaderId = new BlockHeaderId(*blockHeaderId);
	this->list = new ArrayList<VoteCandidate>();
}

VotingBlockStatus::~VotingBlockStatus() {
	delete this->blockHeaderId;

	this->list->deleteElements();
	delete this->list;
}

int VotingBlockStatus::binarySize() const {
	int total = sizeof(this->height);
	total += this->blockHeaderId->binarySize();

	total += sizeof(uint32_t);
	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		VoteCandidate* candidate = this->list->get(i);

		total += candidate->binarySize();
	}

	return total;
}

void VotingBlockStatus::toBinary(ByteBuffer *out) const {
	out->putLong(this->height);
	this->blockHeaderId->toBinary(out);

	int maxLoop = this->list->size();
	out->putInt(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		VoteCandidate* candidate = this->list->get(i);

		candidate->toBinary(out);
	}
}

VotingBlockStatus* VotingBlockStatus::createFromBinary(ByteBuffer *in) {
	uint64_t height = in->getLong();
	BlockHeaderId* blockHeaderId = BlockHeaderId::fromBinary(in); __STP(blockHeaderId);

	VotingBlockStatus* ret = new VotingBlockStatus(height, blockHeaderId);

	int maxLoop = in->getInt();
	for(int i = 0; i != maxLoop; ++i){
		VoteCandidate* candidate = VoteCandidate::createFromBinary(in);

		ret->list->addElement(candidate);
	}

	return ret;
}

VotingBlockStatus* VotingBlockStatus::toVotingBlockStatus(const BlockHeader *header, const ArrayList<const VoteTicket> *list) {
	uint64_t height = header->getHeight();
	const BlockHeaderId* blockHeaderId = header->getId();

	VotingBlockStatus* status = new VotingBlockStatus(height, blockHeaderId);

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		const VoteTicket* ticket = list->get(i);

		const UtxoId* utxoId = ticket->getUtxoId();
		const NodeIdentifier* nodeId = ticket->getNodeIdentifier();
		const AddressDescriptor* adddressDesc = ticket->getAddressDescriptor();
		BalanceUnit ticketPrice = ticket->getTicketProce();

		const AddressDescriptor* voterAdddressDesc = ticket->getVoterAddressDesc();
		assert(voterAdddressDesc != nullptr);

		VoteCandidate* candidate = new VoteCandidate(utxoId, nodeId);
		candidate->setAddressDescriptor(adddressDesc);
		candidate->setTicketPrice(ticketPrice);
		candidate->setVoterAddressDescriptor(voterAdddressDesc);


		status->addVoteCandidate(candidate);
	}

	return status;
}

void VotingBlockStatus::addVoteCandidate(VoteCandidate *candidate) noexcept {
	this->list->addElement(candidate);
}

IBlockObject* VotingBlockStatus::copyData() const noexcept {
	return new VotingBlockStatus(*this);
}

// test method
int VotingBlockStatus::countUtxo(const UtxoId *utxoId) const noexcept {
	int count = 0;

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		const VoteCandidate* ticket = list->get(i);
		const UtxoId* u = ticket->getUtxoId();

		if(u->equals(utxoId)){
			count++;
		}
	}

	return count;
}

const VoteCandidate* VotingBlockStatus::getVoteCandidate(const UtxoId *votedUtxoId) const noexcept {
	const VoteCandidate* ret = nullptr;

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		const VoteCandidate* ticket = list->get(i);
		const UtxoId* u = ticket->getUtxoId();

		if(u->equals(votedUtxoId)){
			ret = ticket;
			break;
		}
	}

	return ret;
}

bool VotingBlockStatus::hasVoteCandidate(const UtxoId* votedUtxoId) const noexcept {
	const VoteCandidate* candidate = getVoteCandidate(votedUtxoId);

	return candidate != nullptr;
}

} /* namespace codablecash */
