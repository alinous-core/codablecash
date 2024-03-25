/*
 * VotingBlockStatus.h
 *
 *  Created on: 2023/04/28
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_CONTEXT_FINALIZER_VOTINGBLOCKSTATUS_H_
#define BC_STATUS_CACHE_CONTEXT_FINALIZER_VOTINGBLOCKSTATUS_H_

#include <cstdint>

#include "base/ArrayList.h"

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class VoteCandidate;
class BlockHeaderId;
class BlockHeader;
class VoteTicket;
class UtxoId;

class VotingBlockStatus : public IBlockObject {
public:
	VotingBlockStatus(const VotingBlockStatus& inst);
	VotingBlockStatus(uint64_t height, const BlockHeaderId* blockHeaderId);
	virtual ~VotingBlockStatus();

	static VotingBlockStatus* toVotingBlockStatus(const BlockHeader *header, const ArrayList<const VoteTicket>* list);

	void addVoteCandidate(VoteCandidate* candidate) noexcept;
	const ArrayList<VoteCandidate>* getCandidatesList() const noexcept {
		return this->list;
	}
	const VoteCandidate* getVoteCandidate(const UtxoId* votedUtxoId) const noexcept;
	bool hasVoteCandidate(const UtxoId* votedUtxoId) const noexcept;

	bool isEmpty() const noexcept {
		return this->list->isEmpty();
	}

	int countUtxo(const UtxoId* utxoId) const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static VotingBlockStatus* createFromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	const BlockHeaderId* getBlockHeaderId() const noexcept {
		return this->blockHeaderId;
	}

	uint64_t getHeight() const noexcept {
		return this->height;
	}

private:
	uint64_t height;
	BlockHeaderId* blockHeaderId;

	ArrayList<VoteCandidate>* list;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_CONTEXT_FINALIZER_VOTINGBLOCKSTATUS_H_ */
