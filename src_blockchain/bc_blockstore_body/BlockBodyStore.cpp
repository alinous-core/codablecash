/*
 * BlockBodyStore.cpp
 *
 *  Created on: 2023/03/01
 *      Author: iizuka
 */

#include "bc_blockstore_body/BlockBodyStore.h"


#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "base_io/File.h"

#include "filestore_variable_block/VariableBlockFileStore.h"

#include "filestore_block/IBlockHandle.h"

#include "base_io/ByteBuffer.h"

#include "bc_block_body/BlockBody.h"

namespace codablecash {

BlockBodyStore::BlockBodyStore(int index, const File* baseDir, DiskCacheManager* cacheManager) {
	this->index = index;
	this->baseDir = new File(*baseDir);
	this->cacheManager = cacheManager;
	this->store = nullptr;
}

BlockBodyStore::~BlockBodyStore() {
	close();
	delete this->baseDir;
}

bool BlockBodyStore::exists() const noexcept {
	File f = getStoreFile();
	File hf = getStoreHeaderFile();

	bool fb = f.exists();
	bool hfb = hf.exists();

	return fb && hfb;
}

void BlockBodyStore::create() {
	UnicodeString fileName(BlockBodyStore::FILE_NAME);
	addIdex2String(&fileName);

	UnicodeString* dir = this->baseDir->getAbsolutePath();
	StackRelease<UnicodeString> __st_dir(dir);

	VariableBlockFileStore tmpStore(dir, &fileName, this->cacheManager);
	tmpStore.createStore(true, 1024, 64);

	tmpStore.open(false);
	IBlockHandle* handle = tmpStore.alloc(10);
	delete handle;
	tmpStore.close();
}

void BlockBodyStore::open() {
	UnicodeString fileName(BlockBodyStore::FILE_NAME);
	addIdex2String(&fileName);

	UnicodeString* dir = this->baseDir->getAbsolutePath();
	StackRelease<UnicodeString> __st_dir(dir);

	this->store = new VariableBlockFileStore(dir, &fileName, this->cacheManager);

	this->store->open(false);
}

void BlockBodyStore::close() noexcept {
	if(this->store != nullptr){
		this->store->close();
		delete this->store, this->store = nullptr;
	}
}

uint64_t BlockBodyStore::storeBody(const BlockBody *body) {
	int size = body->binarySize();

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	body->toBinary(buff);

	buff->position(0);
	assert(buff->capacity() == size);

	IBlockHandle* handle = this->store->alloc(size); __STP(handle);
	handle->write((const char*)buff->array(), size);

	uint64_t fpos = handle->getFpos();
	return fpos;
}

BlockBody* BlockBodyStore::loadBody(uint64_t fpos) {
	IBlockHandle* handle = this->store->get(fpos); __STP(handle);

	ByteBuffer* buff = handle->getBuffer();

	BlockBody* block = BlockBody::fromBinary(buff);

	return block;
}

void BlockBodyStore::removeBody(uint64_t fpos) {
	IBlockHandle* handle = this->store->get(fpos); __STP(handle);
	handle->removeBlocks();
}

File BlockBodyStore::getStoreFile() const noexcept {
	UnicodeString binFileName(BlockBodyStore::FILE_NAME);
	addIdex2String(&binFileName);

	binFileName.append(L".bin");

	File* f = this->baseDir->get(&binFileName);
	StackRelease<File> __st_f(f);

	return *f;
}

File BlockBodyStore::getStoreHeaderFile() const noexcept {
	UnicodeString binFileName(BlockBodyStore::FILE_NAME);
	addIdex2String(&binFileName);

	binFileName.append(L"-header.bin");

	File* f = this->baseDir->get(&binFileName);
	StackRelease<File> __st_f(f);

	return *f;
}

void BlockBodyStore::addIdex2String(UnicodeString *str) const noexcept {
	char* buff = new char[32];
	Mem::memset(buff, 0, 32);
	StackArrayRelease<char> __ST_buff(buff);

	::sprintf(buff, "%08d", this->index);
	UnicodeString buffstr(buff);
	str->append(&buffstr);
}

} /* namespace codablecash */
