/*
 * BlockBodyStore.cpp
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#include "bc_blockstore_body/BlockBodyStoreManager.h"
#include "bc_blockstore_body/BlockBodyStoreCache.h"
#include "bc_blockstore_body/BlockBodyStoreCacheElement.h"

#include "base_thread/ConcurrentGate.h"

#include "base_io/File.h"

namespace codablecash {

BlockBodyStoreManager::BlockBodyStoreManager(const File* baseDir, int sectionLimit) {
	this->baseDir = baseDir->get(BlockBodyStoreManager::MAIN_BODY_DIR);

	this->cache = new BlockBodyStoreCache(this->baseDir, sectionLimit);
	this->sectionLimit = sectionLimit;

	this->rwLock = new ConcurrentGate();
}

BlockBodyStoreManager::~BlockBodyStoreManager() {
	delete this->cache;
	delete this->baseDir;
	delete this->rwLock;
}

void BlockBodyStoreManager::addBlockBody(const BlockBody *body, uint64_t height) {
	StackWriteLock __lock(this->rwLock, __FILE__, __LINE__);

	BlockBodyStoreCacheElement* cache = this->cache->getBodyStoreCacheElement(height, true);
	cache->addBody(body);
}

BlockBody* BlockBodyStoreManager::getBlockBody(const BlockMerkleRoot *root, uint64_t height) {
	StackReadLock __lock(this->rwLock, __FILE__, __LINE__);

	BlockBodyStoreCacheElement* cache = this->cache->getBodyStoreCacheElement(height, false);
	return cache != nullptr ? cache->getBlockBody(root) : nullptr;
}

void BlockBodyStoreManager::removeBlockBody(const BlockMerkleRoot *root,	uint64_t height) {
	StackWriteLock __lock(this->rwLock, __FILE__, __LINE__);

	BlockBodyStoreCacheElement* cache = this->cache->getBodyStoreCacheElement(height, false);
	if(cache != nullptr){
		cache->removeBlock(root, height);
	}
}

} /* namespace codablecash */
