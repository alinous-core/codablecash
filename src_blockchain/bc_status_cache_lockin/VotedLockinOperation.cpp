/*
 * VotedLockonOperation.cpp
 *
 *  Created on: 2024/02/11
 *      Author: iizuka
 */

#include "bc_status_cache_lockin/VotedLockinOperation.h"

#include "bc_status_cache_context/IStatusCacheContext.h"

#include "bc_status_cache_context_finalizer/VoterStatusMappedCacheContext.h"

#include "base/HashMap.h"

#include "bc/CodablecashSystemParam.h"

#include "bc_finalizer/VoterEntry.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_block/BlockHeader.h"


namespace codablecash {

VotedLockinOperation::VotedLockinOperation(const VotedLockinOperation &inst)
		: AbstarctVoteLockinOperation(inst) {
}

VotedLockinOperation::VotedLockinOperation()
		: AbstarctVoteLockinOperation(AbstarctVoteLockinOperation::TYPE_VOTED) {

}

VotedLockinOperation::~VotedLockinOperation() {

}

int VotedLockinOperation::binarySize() const {
	int total = AbstarctVoteLockinOperation::binarySize();

	return total;
}

void VotedLockinOperation::toBinary(ByteBuffer *out) const {
	AbstarctVoteLockinOperation::toBinary(out);

}

void VotedLockinOperation::fromBinary(ByteBuffer *in) {
	AbstarctVoteLockinOperation::fromBinary(in);
}

void VotedLockinOperation::apply(const BlockHeader *header, IStatusCacheContext *context) {
	const CodablecashSystemParam* params = context->getConfig();
	int extendCount = params->getVoteExtendCapacityCount(header->getHeight());

	VoterStatusMappedCacheContext* voterStatus = context->getVoterStatusCacheContext();

	const HashMap<NodeIdentifier, VoterEntry>* votersMap = voterStatus->getVotersMap();
	VoterEntry* entry = votersMap->get(this->nodeId);
	assert(entry != nullptr);

	entry->handleVoted(extendCount);
}

IBlockObject* VotedLockinOperation::copyData() const noexcept {
	return new VotedLockinOperation(*this);
}

bool VotedLockinOperation::dataEquals(const AbstractLockinOperation *op) const noexcept {
	return AbstarctVoteLockinOperation::dataEquals(op);
}

} /* namespace codablecash */
