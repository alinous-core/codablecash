/*
 * BlockHeaderStore.h
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_BLOCKHEADERSTORE_H_
#define BC_BLOCKSTORE_BLOCKHEADERSTORE_H_

#include "base/ArrayList.h"
#include <cstdint>

namespace alinous {
class File;
class ConcurrentGate;
}
using namespace alinous;

namespace codablecash {

class BlockHeaderStoreCache;
class BlockHeader;
class BlockHeaderId;
class IHeaderRemovalNotifier;

class BlockHeaderStoreManager {
public:
	static const constexpr wchar_t* MAIN_HEADER_DIR{L"mainheader"};

	BlockHeaderStoreManager(const File* baseDir, int sectionLimit);
	virtual ~BlockHeaderStoreManager();

	void addHeader(const BlockHeader* header);
	ArrayList<BlockHeader>* getBlocksAtHeight(uint64_t height);
	ArrayList<BlockHeader>* getChildrenOf(const BlockHeaderId *headerId, uint64_t height);

	void removeHeader(BlockHeaderId* hash, uint64_t height);

	BlockHeader* getHeader(const BlockHeaderId* headerId, uint64_t height);

	BlockHeader* getNBlocksBefore(const BlockHeaderId *headerId, uint64_t height, int voteBeforeNBlocks);

	void finalize(uint64_t height, const BlockHeaderId* headerId, IHeaderRemovalNotifier* notifier);

	bool isEmpty() const noexcept;

private:
	ArrayList<BlockHeader>* __getBlocksAtHeight(uint64_t height);

private:
	File* baseDir;
	int sectionLimit;

	BlockHeaderStoreCache* cache;

	ConcurrentGate* rwLock;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_BLOCKHEADERSTORE_H_ */
