/*
 * VotedHeaderIdGroup.cpp
 *
 *  Created on: 2023/05/06
 *      Author: iizuka
 */

#include "bc_block_vote/VotedHeaderIdGroup.h"

#include "bc_block/BlockHeaderId.h"
#include "bc_block/BlockHeader.h"

#include "bc_trx/TransactionId.h"

#include "base/StackRelease.h"


namespace codablecash {

VotedHeaderIdGroup::VotedHeaderIdGroup(const BlockHeaderId* headerId) {
	this->votedHeaderId = dynamic_cast<BlockHeaderId*>(headerId->copyData());
	this->trxIdList = new ArrayList<TransactionId>();
}

VotedHeaderIdGroup::~VotedHeaderIdGroup() {
	delete this->votedHeaderId;

	this->trxIdList->deleteElements();
	delete this->trxIdList;
}

void VotedHeaderIdGroup::add(const TransactionId *trxId) noexcept {
	this->trxIdList->addElement(new TransactionId(*trxId));
}

int VotedHeaderIdGroup::binarySize() const {
	int total = this->votedHeaderId->binarySize();

	int maxLoop = this->trxIdList->size();
	total += sizeof(uint8_t);

	for(int i = 0; i != maxLoop; ++i){
		TransactionId* id = this->trxIdList->get(i);

		total += id->binarySize();
	}

	return total;
}

void VotedHeaderIdGroup::toBinary(ByteBuffer *out) const {
	this->votedHeaderId->toBinary(out);

	int maxLoop = this->trxIdList->size();
	out->put(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		TransactionId* id = this->trxIdList->get(i);

		id->toBinary(out);
	}
}

VotedHeaderIdGroup* VotedHeaderIdGroup::createFromBinary(ByteBuffer *in) {
	BlockHeaderId* headerId = BlockHeaderId::fromBinary(in); __STP(headerId);

	VotedHeaderIdGroup* group = new VotedHeaderIdGroup(headerId);

	int maxLoop = in->get();
	for(int i = 0; i != maxLoop; ++i){
		TransactionId* id = TransactionId::fromBinary(in); __STP(id);

		group->add(id);
	}

	return group;
}

void VotedHeaderIdGroup::buildVoteTransactionIdCertificate(const Block *block, IVoteTransactionIdCertificateBuilder *callback) const {
	int maxLoop = this->trxIdList->size();
	for(int i = 0; i != maxLoop; ++i){
		TransactionId* id = this->trxIdList->get(i);

		callback->detect(block, this->votedHeaderId, id);
	}
}

void VotedHeaderIdGroup::visitVoteTransactionIdCertificate(IVoteTransactionIdCertificatevisitor *visitor) const {
	int maxLoop = this->trxIdList->size();
	for(int i = 0; i != maxLoop; ++i){
		TransactionId* id = this->trxIdList->get(i);

		visitor->visit(this->votedHeaderId, id);
	}
}

} /* namespace codablecash */
