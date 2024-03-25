/*
 * BlockHeaderStore.cpp
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */


#include "bc_blockstore_header/BlockHeaderStoreManager.h"
#include "bc_blockstore_header/BlockHeaderStoreCache.h"
#include "bc_blockstore_header/BlockHeaderStoreCacheElement.h"
#include "bc_blockstore_header/BlockHeaderNotFoundException.h"

#include "bc_block/BlockHeader.h"
#include "bc_block/BlockHeaderId.h"

#include "base_io/File.h"

#include "base_thread/ConcurrentGate.h"

#include "base/StackRelease.h"

#include "bc/ExceptionThrower.h"

namespace codablecash {

BlockHeaderStoreManager::BlockHeaderStoreManager(const File* baseDir, int sectionLimit) {
	this->baseDir = baseDir->get(BlockHeaderStoreManager::MAIN_HEADER_DIR);

	this->cache = new BlockHeaderStoreCache(this->baseDir, sectionLimit);
	this->sectionLimit = sectionLimit;

	this->rwLock = new ConcurrentGate();
}

BlockHeaderStoreManager::~BlockHeaderStoreManager() {
	delete this->cache;
	delete this->baseDir;
	delete this->rwLock;
}

void BlockHeaderStoreManager::addHeader(const BlockHeader *header) {
	StackWriteLock __lock(this->rwLock);

	uint64_t height = header->getHeight();
	BlockHeaderStoreCacheElement* cache = this->cache->getHeaderStoreCacheElement(height, true);

	cache->addHeader(header);
}

ArrayList<BlockHeader>* BlockHeaderStoreManager::getBlocksAtHeight(uint64_t height) {
	StackReadLock __lock(this->rwLock);

	return __getBlocksAtHeight(height);
}

ArrayList<BlockHeader>* BlockHeaderStoreManager::__getBlocksAtHeight(uint64_t height) {
	BlockHeaderStoreCacheElement* cache = this->cache->getHeaderStoreCacheElement(height, false);
	return cache != nullptr ? cache->getHeadersAtHeight(height) : nullptr;
}

bool BlockHeaderStoreManager::isEmpty() const noexcept {
	BlockHeaderStoreCacheElement* cache = this->cache->getHeaderStoreCacheElement(1, false);
	return cache == nullptr;
}

ArrayList<BlockHeader>* BlockHeaderStoreManager::getChildrenOf(const BlockHeaderId *headerId, uint64_t height) {
	StackReadLock __lock(this->rwLock);

	BlockHeaderStoreCacheElement* cache = this->cache->getHeaderStoreCacheElement(height, false);
	return cache != nullptr ? cache->getChildrenOf(headerId, height) : new ArrayList<BlockHeader>();
}


void BlockHeaderStoreManager::removeHeader(BlockHeaderId *hash, uint64_t height) {
	StackWriteLock __lock(this->rwLock);

	BlockHeaderStoreCacheElement* cache = this->cache->getHeaderStoreCacheElement(height, false);
	if(cache == nullptr){
		return;
	}

	cache->removeBlock(hash, height);
}

BlockHeader* BlockHeaderStoreManager::getHeader(const BlockHeaderId *headerId, uint64_t height) {
	StackReadLock __lock(this->rwLock);

	if(height < 1 || headerId->bufferIsNull()){
		return nullptr;
	}

	ArrayList<BlockHeader>* list = __getBlocksAtHeight(height); __STP(list);
	if(list == nullptr){
		return nullptr;
	}
	list->setDeleteOnExit();

	BlockHeader* ret = nullptr;

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		BlockHeader* header = list->get(i);
		const BlockHeaderId* id = header->getId();
		if(id->equals(headerId)){
			ret = list->remove(i);
			break;
		}
	}

	return ret;
}

BlockHeader* BlockHeaderStoreManager::getNBlocksBefore(const BlockHeaderId *headerId, uint64_t height, int voteBeforeNBlocks) {
	StackReadLock __lock(this->rwLock);

	BlockHeader* currentHeader = getHeader(headerId, height);

	// check currentHeader is not null
#ifdef __DEBUG__
	assert(currentHeader != nullptr);
#endif
	ExceptionThrower<BlockHeaderNotFoundException>::throwExceptionIfCondition(currentHeader == nullptr, L"The currentHeader does not found.", __FILE__, __LINE__);

	uint64_t currentHeight = height;
	for(int i = 0; i != voteBeforeNBlocks; ++i){
		 __STP(currentHeader);
		currentHeader = getHeader(currentHeader->getLastHeaderId(), currentHeader->getHeight() - 1);
	}

	return currentHeader;
}

void BlockHeaderStoreManager::finalize(uint64_t height,	const BlockHeaderId *headerId, IHeaderRemovalNotifier* notifier) {
	StackWriteLock __lock(this->rwLock);

	BlockHeaderStoreCacheElement* cache = this->cache->getHeaderStoreCacheElement(height, false);
	cache->finalize(height, headerId, notifier);
}

} /* namespace codablecash */
