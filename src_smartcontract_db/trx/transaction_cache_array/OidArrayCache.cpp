/*
 * OidArrayCache.cpp
 *
 *  Created on: 2021/06/14
 *      Author: iizuka
 */

#include "trx/transaction_cache_array/OidArrayCache.h"

#include "filestore_block/BlockFileStore.h"

namespace codablecash {

OidArrayCache::OidArrayCache() {
	this->blockStore = nullptr;

}

OidArrayCache::~OidArrayCache() {
	shutdown();
}

void OidArrayCache::init(UnicodeString* dir, UnicodeString* name, DiskCacheManager* cacheManager) {
	shutdown();

	this->blockStore = new BlockFileStore(dir, name, cacheManager);
	this->blockStore->createStore(true, 256, 256);

	this->blockStore->open(false);
}

void OidArrayCache::shutdown() {
	if(this->blockStore != nullptr){
		if(this->blockStore->isOpened()){
			this->blockStore->close();
			this->blockStore->deleteFiles();
		}

		delete this->blockStore;
		this->blockStore = nullptr;
	}
}

} /* namespace codablecash */
