/*
 * VotingBlockStatusRepository.h
 *
 *  Created on: 2023/04/28
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_CONTEXT_FINALIZER_VOTINGBLOCKSTATUSREPOSITORY_H_
#define BC_STATUS_CACHE_CONTEXT_FINALIZER_VOTINGBLOCKSTATUSREPOSITORY_H_
#include <cstdint>

namespace alinous {
class File;
class DiskCacheManager;
class Btree;
class BtreeScanner;
}
using namespace alinous;

namespace codablecash {

class VotingBlockStatus;
class BlockHeaderId;

class VotingBlockStatusRepository {
public:
	static const constexpr wchar_t* NAME_VOTE_STATUS_STORE{L"voteStatusStore"};

	VotingBlockStatusRepository(const File* baseDir);
	virtual ~VotingBlockStatusRepository();

	void init();

	void open();
	void close();

	void storeVotingBlockStatus(const VotingBlockStatus* status);
	VotingBlockStatus* getVotingBlockStatus(const BlockHeaderId* blockHeaderId);

	BtreeScanner* getBtreeScanner();

	void clean(uint64_t height);

private:
	void __close(Btree** tree) noexcept;

private:
	File* baseDir;
	DiskCacheManager* cacheManager;

	Btree* voteStatusStore;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_CONTEXT_FINALIZER_VOTINGBLOCKSTATUSREPOSITORY_H_ */
