/*
 * CachedStatusCacheContext.h
 *
 *  Created on: 2023/04/26
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_CONTEXT_CACHEDSTATUSCACHECONTEXT_H_
#define BC_STATUS_CACHE_CONTEXT_CACHEDSTATUSCACHECONTEXT_H_

#include <cstdint>

#include "bc_status_cache_context/StatusCacheContext.h"

#include "bc_finalizer/VoterEntry.h"
namespace alinous {
class File;
class ConcurrentGate;
class BlockchainStatusCache;
}
using namespace alinous;

namespace codablecash {

class CachedStatusCache;
class CodablecashConfig;
class TransactionContextCache;
class UtxoCacheContext;
class VoterStatusCacheContext;
class AbstractBlockchainTransaction;

class CachedStatusCacheContext : public StatusCacheContext {
public:
	static const constexpr wchar_t* CACHED_CONTEXT_CACHE_PREFIX{L"cachedcontextcache"};

	explicit CachedStatusCacheContext(const CachedStatusCache* cache, const CodablecashConfig* config, const File* tmpCacheBaseDir,
			uint16_t zone, ConcurrentGate* rwLock, BlockchainStatusCache* statusCache, CodablecashBlockchain* blockchain);
	virtual ~CachedStatusCacheContext();

	virtual void close();

	virtual ArrayList<VoterEntry, VoterEntry::VoteCompare>* getVoterEntries() const;

	virtual VotingBlockStatus* getVotingBlockStatus(const BlockHeaderId *blockHeaderId);

	virtual const VoterEntry* getVoterEntry(const NodeIdentifier* nodeId) const noexcept;

	virtual AbstractUtxo* getUtxo(const UtxoId* utxoId) const;

	virtual const CachedStatusCache* getCachedStatusCache() const noexcept {
		return this->cache;
	}

	virtual void loadInitialVotersData();

	virtual uint64_t getAnalyzedHeight() const noexcept;

protected:
	virtual void putUtxo(const AbstractUtxo* utxo, const AbstractBlockchainTransaction *trx, const BlockHeader *header);
	virtual void removeUtxo(const AbstractUtxoReference *utxoref, const AbstractBlockchainTransaction *trx);

private:
	const CachedStatusCache* cache;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_CONTEXT_CACHEDSTATUSCACHECONTEXT_H_ */
