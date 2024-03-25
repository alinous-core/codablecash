/*
 * FinalizedDataCache.h
 *
 *  Created on: 2023/05/08
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_DATA_FINALIZEDDATACACHE_H_
#define BC_STATUS_CACHE_DATA_FINALIZEDDATACACHE_H_

namespace alinous {
class File;
}
using namespace alinous;

namespace codablecash {

class BlockHeader;
class BlockBody;
class FinalizedUtxoRepository;
class FinalizedVoterRepository;
class AbstractBlockchainTransaction;
class RegisterVotePoolTransaction;
class RegisterTicketTransaction;
class VoteBlockTransaction;
class IStatusCacheContext;
class UtxoData;
class UtxoId;


class FinalizedDataCache {
public:
	static const constexpr wchar_t* NAME_FINALIZED_DATA{L"finalized_data"};

	explicit FinalizedDataCache(const File* baseDir);
	virtual ~FinalizedDataCache();

	void initBlank();

	void open();
	void close();

	void importBlockData(const BlockHeader* header, const BlockBody* body, IStatusCacheContext* context);
	void writeBackVoterEntries(IStatusCacheContext* context);

	FinalizedVoterRepository* getFinalizedVoterRepository() const noexcept {
		return this->voterRepo;
	}

	UtxoData* findUtxo(const UtxoId* utxoId) const;

private:
	void importControlTransactions(const BlockHeader* header, const BlockBody* body, IStatusCacheContext* context);
	void importRegisterVotePoolTransaction(const BlockHeader* header, const BlockBody* body, const RegisterVotePoolTransaction* trx, IStatusCacheContext* context);
	void importRegisterTicketTransaction(const BlockHeader* header, const BlockBody* body, const RegisterTicketTransaction* trx, IStatusCacheContext* context);
	void importVoteBlockTransaction(const BlockHeader* header, const BlockBody* body, const VoteBlockTransaction* trx, IStatusCacheContext* context);

	void importBalanceTransactions(const BlockHeader* header, const BlockBody* body);
	void importInterChainCommunicationTransactions(const BlockHeader* header, const BlockBody* body);
	void importSmartcontractTransactions(const BlockHeader* header, const BlockBody* body);

	void importRewardBaseTransactions(const BlockHeader* header, const BlockBody* body);

	void importTransactionUtxos(const AbstractBlockchainTransaction* trx, const BlockHeader *header);

private:
	File* baseDir;

	FinalizedUtxoRepository* utxoRepo;
	FinalizedVoterRepository* voterRepo;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_DATA_FINALIZEDDATACACHE_H_ */
