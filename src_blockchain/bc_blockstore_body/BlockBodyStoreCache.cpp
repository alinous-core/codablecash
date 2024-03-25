/*
 * BlockBodyStoreCache.cpp
 *
 *  Created on: 2023/03/01
 *      Author: iizuka
 */

#include "bc_blockstore_body/BlockBodyStoreCache.h"
#include "bc_blockstore_body/BlockBodyStoreCacheElement.h"

#include "base/RawCompare.h"
#include "base/StackRelease.h"

namespace codablecash {

BlockBodyStoreCache::BlockBodyStoreCache(const File* baseDir, int sectionLimit) {
	this->baseDir = baseDir;
	this->sectionLimit = sectionLimit;
	this->maxCache = 2;
	this->list = new RawLinkedList<BlockBodyStoreCacheElement>();
}

BlockBodyStoreCache::~BlockBodyStoreCache() {
	RawLinkedList<BlockBodyStoreCacheElement,RawCompare>::Iterator it = this->list->iterator();
	while(it.hasNext()){
		BlockBodyStoreCacheElement* element = it.next();
		delete element;
	}

	delete this->list;

	this->baseDir = nullptr;
}

BlockBodyStoreCacheElement* BlockBodyStoreCache::getBodyStoreCacheElement(uint64_t height, bool create) {
	uint64_t index = height / this->sectionLimit;

	BlockBodyStoreCacheElement* cache = findCache(index);
	if(cache != nullptr){
		return cache;
	}

	if(this->list->size() == this->maxCache){
		RawLinkedList<BlockBodyStoreCacheElement,RawCompare>::Element* lastElement =  this->list->getLastElement();

		BlockBodyStoreCacheElement* lastCache = lastElement->data; __STP(lastCache);
		lastCache->close();

		this->list->remove(lastElement);
	}

	cache = new BlockBodyStoreCacheElement(this->baseDir, index); __STP(cache);
	if(!cache->exist() && !create){
		return nullptr;
	}

	if(!cache->exist()){
		cache->create();
	}

	cache->init(); // open

	this->list->add(0, cache);

	return __STP_MV(cache);
}

BlockBodyStoreCacheElement* BlockBodyStoreCache::findCache(	uint64_t index) noexcept {
	BlockBodyStoreCacheElement* ret = nullptr;

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		BlockBodyStoreCacheElement* cache = this->list->get(i);

		if(index == cache->getIndex()){
			this->list->moveElementToTop(i);
			ret = cache;
			break;
		}
	}

	return ret;
}

} /* namespace codablecash */
