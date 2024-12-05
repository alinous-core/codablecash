/*
 * VoterStatusCacheContext.h
 *
 *  Created on: 2023/04/25
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_CONTEXT_VOTERSTATUSCACHECONTEXT_H_
#define BC_STATUS_CACHE_CONTEXT_VOTERSTATUSCACHECONTEXT_H_

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
class ILockinManager;
class CodablecashSystemParam;

class VoterStatusCacheContext {
public:
	explicit VoterStatusCacheContext(const File* baseDir);
	virtual ~VoterStatusCacheContext();

	void init();

	void initBlank();
	void open();
	void close();

	void storeVotingBlockStatus(const VotingBlockStatus* status);
	VotingBlockStatus* getVotingBlockStatus(const BlockHeaderId* blockHeaderId);

	void importRepo(const VoterStatusCacheContext* other);

	void handleVote(VotingBlockStatus* status, const UtxoId* utxoId);

	void clean(uint64_t height);

protected:
	VotingBlockStatusRepository* repo;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_CONTEXT_VOTERSTATUSCACHECONTEXT_H_ */
