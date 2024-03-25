/*
 * VoterStatusCacheContext.h
 *
 *  Created on: 2023/04/25
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_CONTEXT_VOTERSTATUSCACHECONTEXT_H_
#define BC_STATUS_CACHE_CONTEXT_VOTERSTATUSCACHECONTEXT_H_

#include "base/HashMap.h"
#include "base/ArrayList.h"

#include "bc_finalizer/VoterEntry.h"


namespace alinous {
class File;
}
using namespace alinous;

namespace codablecash {

class VoterEntry;
class NodeIdentifier;
class BlockchainStatusCache;
class VotingBlockStatusRepository;
class VotingBlockStatus;
class BlockHeaderId;

class VoterStatusCacheContext {
public:
	explicit VoterStatusCacheContext(const File* baseDir);
	virtual ~VoterStatusCacheContext();

	void init();

	void addNewVoter(VoterEntry* entry);
	void listEntries(ArrayList<VoterEntry, VoterEntry::VoteCompare>* list) const noexcept;

	void storeVotingBlockStatus(const VotingBlockStatus* status);
	void removeVotedTicket(const VotingBlockStatus* status);
	VotingBlockStatus* getVotingBlockStatus(const BlockHeaderId* blockHeaderId);

	void importMap(const VoterStatusCacheContext* other);
	void importRepo(const VoterStatusCacheContext* other);

	const VoterEntry* getVoterEntry(const NodeIdentifier *nodeId) const noexcept;

	void loadFinalyzedVoters(uint16_t zone, BlockchainStatusCache* statusCache);
	void loadLastCache(const VoterStatusCacheContext* lastVoterCache);

	void handleVotedStatus(VotingBlockStatus* status, int missingLimit, int extendCount);
	void handleVote(VotingBlockStatus* status, const UtxoId* utxoId);

	const HashMap<NodeIdentifier, VoterEntry>* getVotersMap() const noexcept {
		return this->votersMap;
	}

private:
	HashMap<NodeIdentifier, VoterEntry>* votersMap;
	VotingBlockStatusRepository* repo;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_CONTEXT_VOTERSTATUSCACHECONTEXT_H_ */
