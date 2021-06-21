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
	this->firstIndexFpos = 0;
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

	this->firstIndexFpos = handle->getFpos();
	element.setFpos(this->firstIndexFpos);
	element.toBinary(buff);

	buff->position(0);
	const char* array = (const char*)buff->array();

	BlockHandle* handlew = this->blockStore->get(handle->getFpos()); __STP(handlew);
	handlew->write(array, blkSize);
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

void OidArrayCache::add(int index, const CdbOid* oid) {
	uint64_t fpos = getIndexFpos(index);

}

uint64_t OidArrayCache::getIndexFpos(int index) {
	int mod = index % INDEX_ELEMENT_SIZE;
	int page = index / INDEX_ELEMENT_SIZE;

	OidArrayIndexElement* lastElement = loadOidArrayIndexElement(this->firstIndexFpos);

	for(int i = 1; i <= page; ++i){
		uint64_t nextFpos = lastElement->getNextFpos();
		if(nextFpos == 0){
			// create index
			nextFpos = createIndexElement();

			lastElement->setNextFpos(nextFpos);
			saveIndexElement(lastElement);
		}

		OidArrayIndexElement* element = loadOidArrayIndexElement(nextFpos);

		delete lastElement;
		lastElement = element;
	}

	uint64_t ret = lastElement->getElementPos(mod);
	if(ret == 0){
		// create array

	}

	delete lastElement;

	return ret;
}

OidArrayIndexElement* OidArrayCache::loadOidArrayIndexElement(uint64_t fpos) {
	BlockHandle* handle = this->blockStore->get(fpos); __STP(handle);

	ByteBuffer* buff = handle->getBuffer();
	OidArrayIndexElement* element = OidArrayIndexElement::fromBinary(buff);

	return element;
}

uint64_t OidArrayCache::createIndexElement() {
	OidArrayIndexElement element(INDEX_ELEMENT_SIZE);

	int blkSize = element.blockSize();

	BlockHandle* handle = this->blockStore->alloc(blkSize); __STP(handle);
	ByteBuffer* buff = handle->getBuffer();

	buff->position(0);

	uint64_t pos = handle->getFpos();
	element.setFpos(pos);
	element.toBinary(buff);

	buff->position(0);
	const char* array = (const char*)buff->array();

	BlockHandle* handlew = this->blockStore->get(handle->getFpos()); __STP(handlew);
	handlew->write(array, blkSize);

	return pos;
}

void OidArrayCache::saveIndexElement(OidArrayIndexElement* element) {
	uint64_t fpos = element->getFpos();

	BlockHandle* handle = this->blockStore->get(fpos); __STP(handle);
	ByteBuffer* buff = handle->getBuffer();

	buff->position(0);
	element->toBinary(buff);

	buff->position(0);
	const char* array = (const char*)buff->array();

	int blkSize = element->blockSize();

	BlockHandle* handlew = this->blockStore->get(handle->getFpos()); __STP(handlew);
	handlew->write(array, blkSize);
}

} /* namespace codablecash */
