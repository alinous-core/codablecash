/*
 * VariableBlockFileStore.cpp
 *
 *  Created on: 2023/02/08
 *      Author: iizuka
 */

#include "filestore_variable_block/VariableBlockFileStore.h"
#include "filestore_variable_block/VariableBlockHeader.h"
#include "filestore_variable_block/VariableBlockFileBody.h"
#include "filestore_variable_block/VariableBlock.h"

#include "base/Exception.h"
#include "base/ArrayList.h"
#include "base/StackRelease.h"

#include "filestore_block/exceptions.h"

#include "debug/debugMacros.h"

#include "filestore_variable_block/VariableBlockHandle.h"

#include "base_io/ByteBuffer.h"
namespace alinous {

VariableBlockFileStore::VariableBlockFileStore(const UnicodeString* dir, const UnicodeString* name, DiskCacheManager* cacheManager)
		: FileStore(dir, name, cacheManager) {
	this->header = nullptr;
	this->body = nullptr;
}

VariableBlockFileStore::~VariableBlockFileStore() {
	close();
}

void VariableBlockFileStore::createStore(bool del, uint64_t defaultSize,
			uint64_t blockSize) noexcept (false) {
	createStore(del, defaultSize, blockSize, 1024);
}

void VariableBlockFileStore::createStore(bool del, uint64_t defaultSize
		,uint64_t blockUnitSize, uint64_t extendBlocks) noexcept (false) {
	uint64_t mod = defaultSize % blockUnitSize;
	assert(mod == 0);

	FileStore::createStore(del, defaultSize);
	FileStore::open(false);

	this->header = new VariableBlockHeader(this->headerFile);

	try{
		ERROR_POINT(L"VariableBlockFileStore::createStore")

		this->header->createStore(del, defaultSize, blockUnitSize, extendBlocks);

		this->body = new VariableBlockFileBody(this->file, this->header->getBlockUnitSize());
		this->body->createStore(del, blockUnitSize);
	}
	catch(Exception* e){
		internalClear();
		FileStore::close();

		throw new BlockFileStorageException(L"Failed in creating block file store", e, __FILE__, __LINE__);
	}

	close();
}

void VariableBlockFileStore::internalClear() noexcept {
	if(this->header){
		delete this->header;
		this->header = nullptr;
	}
	if(this->body){
		delete this->body;
		this->body = nullptr;
	}
}

void VariableBlockFileStore::open(bool sync) noexcept (false) {
	ERROR_POINT(L"VariableBlockFileStore::open")

	FileStore::open(sync);

	this->header = new VariableBlockHeader(this->headerFile);

	try{
		this->header->loadFromFile();
		this->body = new VariableBlockFileBody(this->file, this->header->getBlockUnitSize());
	}
	catch(Exception* e){
		internalClear();
		FileStore::close();

		throw new BlockFileStorageException(L"Failed in opening block file store", e, __FILE__, __LINE__);
	}
}

void VariableBlockFileStore::close() noexcept {
	internalClear();

	FileStore::close();
}

void VariableBlockFileStore::extendFile() {
	uint64_t numExtendedBlocks = this->header->extend();

	uint64_t newLength = numExtendedBlocks * this->header->getBlockUnitSize();
	this->body->extend(newLength);

	sync(false);
}

IBlockHandle* VariableBlockFileStore::realloc(uint64_t fpos, uint64_t size) {
	if(this->header->isEmpty() || this->header->availableCapacity() < size){
		// extend file size
		extendFile();
	}

	int64_t sizeRemain = size;

	ArrayList<VariableBlock> list;
	list.setDeleteOnExit();

	// first block
	uint16_t blockUnitSize = this->header->getBlockUnitSize();
	uint64_t blockPos = fpos / blockUnitSize;

	VariableBlock* firstBlock = this->header->reallocFirstMaxFragment(blockPos, sizeRemain);
	sizeRemain -= firstBlock->getUsedSize();
	list.addElement(firstBlock);

	// second
	VariableBlock* lastBlock = firstBlock;
	while(sizeRemain > 0){
		VariableBlock* block = this->header->allocMaxFragment(sizeRemain);
		sizeRemain -= block->getUsedSize();

		lastBlock->setNextfpos(block->getfPos());

		list.addElement(block);
		lastBlock = block;
	}

	assert(sizeRemain == 0);

	int maxLoop = list.size();
	for(int i = 0; i != maxLoop; ++i){
		VariableBlock* block = list.get(i);
		block->writeBack(this->body);
	}

	sync(false);

	return blocksToHandle(&list);
}

IBlockHandle* VariableBlockFileStore::alloc(uint64_t size) {
	if(this->header->isEmpty() || this->header->availableCapacity() < size){
		// extend file size
		extendFile();
	}

	int64_t sizeRemain = size;

	ArrayList<VariableBlock> list;
	list.setDeleteOnExit();

	VariableBlock* lastBlock = nullptr;
	do{
		VariableBlock* block = this->header->allocMaxFragment(sizeRemain);
		sizeRemain -= block->getUsedSize();

		if(lastBlock != nullptr){
			lastBlock->setNextfpos(block->getfPos());
		}

		list.addElement(block);
		lastBlock = block;
	}
	while(sizeRemain > 0);

	assert(sizeRemain == 0);

	int maxLoop = list.size();
	for(int i = 0; i != maxLoop; ++i){
		VariableBlock* block = list.get(i);
		block->writeBack(this->body);
	}

	sync(false);

	return blocksToHandle(&list);
}


IBlockHandle* VariableBlockFileStore::blocksToHandle(ArrayList<VariableBlock> *list) {
	VariableBlockHandle* handle = new VariableBlockHandle(this);
	{
		uint64_t fpos = list->get(0)->getfPos();
		handle->setFpos(fpos);
	}

	// datasize
	int dataSize = 0;
	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		VariableBlock* block = list->get(i);

		dataSize += block->getUsedSize();
	}

	// copy
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(dataSize, true);
	handle->setBuffer(buff);

	for(int i = 0; i != maxLoop; ++i){
		VariableBlock* block = list->get(i);

		int length = block->getUsedSize();
		const char* data = block->getData();

		buff->put((const uint8_t*)data, length);
	}

	buff->position(0);

	return handle;
}


IBlockHandle* VariableBlockFileStore::get(uint64_t fpos) {
	ArrayList<VariableBlock>* list = getBlockList(fpos); __STP(list);
	list->setDeleteOnExit();

	IBlockHandle* handle = blocksToHandle(list);

	return handle;
}

ArrayList<VariableBlock>* VariableBlockFileStore::getBlockList(uint64_t fpos) {
	uint16_t blockUnitSize = this->header->getBlockUnitSize();

	ArrayList<VariableBlock>* list = new ArrayList<VariableBlock>();

	uint64_t __fpos = fpos;

	VariableBlock* block = nullptr;
	do{
		block = VariableBlock::load(this->body, __fpos, blockUnitSize);
		list->addElement(block);

		__fpos = block->getNextPos();
	}while(__fpos != 0);

	return list;
}

void VariableBlockFileStore::sync(bool fsync) {
	this->header->sync(fsync);
	this->body->sync(fsync);
}

} /* namespace alinous */
