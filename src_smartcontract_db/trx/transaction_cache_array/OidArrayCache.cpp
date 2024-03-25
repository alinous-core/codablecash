/*
 * OidArrayCache.cpp
 *
 *  Created on: 2021/06/14
 *      Author: iizuka
 */

#include "trx/transaction_cache_array/OidArrayCache.h"
#include "trx/transaction_cache_array/OidArrayIndexElement.h"
#include "trx/transaction_cache_array/OidArrayCacheElement.h"
#include "trx/transaction_cache_array/OidArrayCacheScanner.h"

#include "filestore_block/BlockFileStore.h"
#include "filestore_block/BlockHandle.h"

#include "engine/CdbOid.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"

namespace codablecash {

OidArrayCache::OidArrayCache(int indexElementSize, int arrayElementSize) {
	this->indexElementSize = indexElementSize;
	this->arrayElementSize = arrayElementSize;

	this->blockStore = nullptr;
	this->firstIndexFpos = 0;
}

OidArrayCache::~OidArrayCache() {
	shutdown();
}

void OidArrayCache::init(const UnicodeString* dir, const UnicodeString* name, DiskCacheManager* cacheManager) {
	shutdown();

	this->blockStore = new BlockFileStore(dir, name, cacheManager);
	this->blockStore->createStore(true, 256, 256);

	this->blockStore->open(false);

	initFirstIndexElement();
}

void OidArrayCache::initFirstIndexElement() {
	OidArrayIndexElement element(this->indexElementSize);

	int blkSize = element.blockSize();

	IBlockHandle* handle = this->blockStore->alloc(blkSize); __STP(handle);
	ByteBuffer* buff = handle->getBuffer();

	buff->position(0);

	this->firstIndexFpos = handle->getFpos();
	element.setFpos(this->firstIndexFpos);
	element.toBinary(buff);

	buff->position(0);
	const char* array = (const char*)buff->array();

	IBlockHandle* handlew = this->blockStore->get(handle->getFpos()); __STP(handlew);
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

void OidArrayCache::add(uint64_t index, const CdbOid* oid) {
	uint64_t fpos = getIndexFpos(index);

//	int mod = index % INDEX_ELEMENT_SIZE;
	OidArrayCacheElement* cache = loadOidArrayElement(fpos);

	while(cache->isFull()){
		uint64_t nextFpos = cache->getNextFpos();
		if(nextFpos == 0){
			OidArrayCacheElement* nextcache = createOidArrayElement();

			cache->setNextFpos(nextcache->getFpos());
			saveOidArrayElement(cache);

			delete cache;
			cache = nextcache;
		}
		else{
			OidArrayCacheElement* nextcache = loadOidArrayElement(nextFpos);

			delete cache;
			cache = nextcache;
		}
	}

	cache->addOid(oid);
	saveOidArrayElement(cache);

	delete cache;
}

uint64_t OidArrayCache::getIndexFpos(int index) {
	int mod = index % this->indexElementSize;
	int page = index / this->indexElementSize;

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
		OidArrayCacheElement* el = createOidArrayElement(); __STP(el);
		uint64_t arFpos = el->getFpos();

		lastElement->setElementPos(mod, arFpos);
		saveIndexElement(lastElement);

		ret = arFpos;
	}

	delete lastElement;

	return ret;
}

OidArrayIndexElement* OidArrayCache::loadOidArrayIndexElement(uint64_t fpos) {
	IBlockHandle* handle = this->blockStore->get(fpos); __STP(handle);

	ByteBuffer* buff = handle->getBuffer();
	OidArrayIndexElement* element = OidArrayIndexElement::fromBinary(buff);

	return element;
}

uint64_t OidArrayCache::createIndexElement() {
	OidArrayIndexElement element(this->indexElementSize);

	int blkSize = element.blockSize();

	IBlockHandle* handle = this->blockStore->alloc(blkSize); __STP(handle);
	ByteBuffer* buff = handle->getBuffer();

	buff->position(0);

	uint64_t pos = handle->getFpos();
	element.setFpos(pos);
	element.toBinary(buff);

	buff->position(0);
	const char* array = (const char*)buff->array();

	IBlockHandle* handlew = this->blockStore->get(handle->getFpos()); __STP(handlew);
	handlew->write(array, blkSize);

	return pos;
}

OidArrayCacheScanner* OidArrayCache::getScanner(int index) {
	OidArrayCacheScanner* scanner = new OidArrayCacheScanner(this);
	scanner->init(index);

	return scanner;
}

void OidArrayCache::saveIndexElement(OidArrayIndexElement* element) {
	uint64_t fpos = element->getFpos();
	int blockSize = element->blockSize();

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(blockSize, true); __STP(buff);
	buff->position(0);
	element->toBinary(buff);

	buff->position(0);
	const char* array = (const char*)buff->array();

	int blkSize = element->blockSize();

	IBlockHandle* handlew = this->blockStore->get(fpos); __STP(handlew);
	handlew->write(array, blkSize);
}

OidArrayCacheElement* OidArrayCache::createOidArrayElement() {
	OidArrayCacheElement* element = new OidArrayCacheElement(this->arrayElementSize);

	int blockSize = element->blockSize();

	IBlockHandle* handle = this->blockStore->alloc(blockSize); __STP(handle);
	ByteBuffer* buff = handle->getBuffer();

	buff->position(0);

	uint64_t pos = handle->getFpos();
	element->setFpos(pos);
	element->toBinary(buff);

	buff->position(0);
	const char* array = (const char*)buff->array();

	IBlockHandle* handlew = this->blockStore->get(handle->getFpos()); __STP(handlew);
	handlew->write(array, blockSize);

	return element;
}

OidArrayCacheElement* OidArrayCache::loadOidArrayElement(uint64_t fpos) {
	IBlockHandle* handle = this->blockStore->get(fpos); __STP(handle);

	ByteBuffer* buff = handle->getBuffer();

	OidArrayCacheElement* element = OidArrayCacheElement::fromBinary(buff, this->arrayElementSize);
	element->setFpos(handle->getFpos());

	return element;
}

void OidArrayCache::saveOidArrayElement(OidArrayCacheElement* element) {
	uint64_t fpos = element->getFpos();
	int blockSize = element->blockSize();

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(blockSize, true); __STP(buff);
	element->toBinary(buff);

	buff->position(0);
	const char* array = (const char*)buff->array();

	IBlockHandle* handlew = this->blockStore->get(fpos); __STP(handlew);
	handlew->write(array, blockSize);
}

} /* namespace codablecash */
