/*
 * MissvoteLockonOperation.cpp
 *
 *  Created on: 2024/02/11
 *      Author: iizuka
 */

#include "bc_status_cache_lockin/MissvoteLockinOperation.h"

#include "bc_status_cache_context/IStatusCacheContext.h"

#include "bc/CodablecashSystemParam.h"

#include "bc_block/BlockHeader.h"

#include "bc_status_cache_context_finalizer/VoterStatusMappedCacheContext.h"

#include "base/HashMap.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_finalizer/VoterEntry.h"


namespace codablecash {

MissvoteLockinOperation::MissvoteLockinOperation(const MissvoteLockinOperation &inst)
		: AbstarctVoteLockinOperation(inst) {

}

MissvoteLockinOperation::MissvoteLockinOperation()
		: AbstarctVoteLockinOperation(AbstarctVoteLockinOperation::TYPE_MISS_VOTED) {

}

MissvoteLockinOperation::~MissvoteLockinOperation() {

}

int MissvoteLockinOperation::binarySize() const {
	int total = AbstarctVoteLockinOperation::binarySize();

	return total;
}

void MissvoteLockinOperation::toBinary(ByteBuffer *out) const {
	AbstarctVoteLockinOperation::toBinary(out);
}

void MissvoteLockinOperation::fromBinary(ByteBuffer *in) {
	AbstarctVoteLockinOperation::fromBinary(in);
}

void MissvoteLockinOperation::apply(const BlockHeader *header, IStatusCacheContext *context) {
	const CodablecashSystemParam* params = context->getConfig();
	int missingLimit = params->getVoteMissingLimit(header->getHeight());

	VoterStatusMappedCacheContext* voterStatus = context->getVoterStatusCacheContext();

	const HashMap<NodeIdentifier, VoterEntry>* votersMap = voterStatus->getVotersMap();
	VoterEntry* entry = votersMap->get(this->nodeId);
	assert(entry != nullptr);

	entry->handleMissed(missingLimit);
}

IBlockObject* MissvoteLockinOperation::copyData() const noexcept {
	return new MissvoteLockinOperation(*this);
}

bool MissvoteLockinOperation::dataEquals(const AbstractLockinOperation *op) const noexcept {
	return AbstarctVoteLockinOperation::dataEquals(op);
}

} /* namespace codablecash */
