/*
 * VoterStatusMappedCacheContext.h
 *
 *  Created on: Nov 4, 2024
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_CONTEXT_FINALIZER_VOTERSTATUSMAPPEDCACHECONTEXT_H_
#define BC_STATUS_CACHE_CONTEXT_FINALIZER_VOTERSTATUSMAPPEDCACHECONTEXT_H_

#include "bc_status_cache_context_finalizer/VoterStatusCacheContext.h"

#include "base/HashMap.h"

namespace codablecash {

class VoterStatusMappedCacheContext : public VoterStatusCacheContext {
public:
	explicit VoterStatusMappedCacheContext(const File* baseDir);
	virtual ~VoterStatusMappedCacheContext();

	void addNewVoter(VoterEntry* entry);

	void listEntries(ArrayList<VoterEntry, VoterEntry::VoteCompare>* list) const noexcept;

	void importMap(const VoterStatusMappedCacheContext* other);

	void loadLastCache(const VoterStatusMappedCacheContext* lastVoterCache);
	void loadFinalyzedVoters(uint16_t zone, BlockchainStatusCache* statusCache);

	void removeVotedTicket(const VotingBlockStatus* status);

	const HashMap<NodeIdentifier, VoterEntry>* getVotersMap() const noexcept {
		return this->votersMap;
	}

	const VoterEntry* getVoterEntry(const NodeIdentifier *nodeId) const noexcept;

	void handleVotedStatus(uint64_t height, VotingBlockStatus* status, int missingLimit, int extendCount, ILockinManager* lockinManager, const CodablecashSystemParam* params);

protected:
	HashMap<NodeIdentifier, VoterEntry>* votersMap;

};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_CONTEXT_FINALIZER_VOTERSTATUSMAPPEDCACHECONTEXT_H_ */
