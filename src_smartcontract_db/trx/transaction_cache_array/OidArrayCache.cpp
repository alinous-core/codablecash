/*
 * OidArrayCache.cpp
 *
 *  Created on: 2021/06/14
 *      Author: iizuka
 */

#include "trx/transaction_cache_array/OidArrayCache.h"
#include "trx/transaction_cache_array/OidArrayIndexElement.h"

#include "filestore_block/BlockFileStore.h"
#include "filestore_block/BlockHandle.h"

#include "engine/CdbOid.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"

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

	initFirstIndexElement();
}

void OidArrayCache::initFirstIndexElement() {
	OidArrayIndexElement element(INDEX_ELEMENT_SIZE);

	int blkSize = element.blockSize();

	BlockHandle* handle = this->blockStore->alloc(blkSize); __STP(handle);
	ByteBuffer* buff = handle->getBuffer();

	buff->position(0);
	element.toBinary(buff);

	buff->position(0);
	const char* array = (const char*)buff->array();

	handle->write(array, blkSize);
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

void OidArrayCache::add(int index, CdbOid* oid) {


}

} /* namespace codablecash */
