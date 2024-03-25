/*
 * VariableBlockHandle.cpp
 *
 *  Created on: 2023/02/08
 *      Author: iizuka
 */

#include "filestore_variable_block/VariableBlockHandle.h"
#include "filestore_variable_block/VariableBlock.h"
#include "filestore_variable_block/VariableBlockFileStore.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"

#include "filestore_variable_block/VariableBlockHeader.h"
namespace alinous {

VariableBlockHandle::VariableBlockHandle(VariableBlockFileStore* store) {
	this->fpos = 0;
	this->store = store;
	this->buffer = nullptr;
}

VariableBlockHandle::~VariableBlockHandle() {
	delete this->buffer;
}

uint64_t VariableBlockHandle::getFpos() const {
	return this->fpos;
}

void VariableBlockHandle::write(const char *bytes, int length) {
	VariableBlockFileBody* body = this->store->getBody();

	ArrayList<VariableBlock>* list = this->store->getBlockList(this->fpos);
	list->setDeleteOnExit();

	// need realloc
	if(needRealloc(list, length)){
		removeBlocks(list);
		delete list;
		list = realloc(length);
		list->setDeleteOnExit();
	}
	 __STP(list);

	 // replace buffer
	 if(this->buffer->capacity() < length){
		 ByteBuffer* b = ByteBuffer::allocateWithEndian(length, true);
		 setBuffer(b);
	 }

	this->buffer->position(0);
	this->buffer->put((const uint8_t*)bytes, length);
	this->buffer->position(0);
	const char* ptr = (const char*)this->buffer->array();

	int writeRemain = length;

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		VariableBlock* block = list->get(i);

		int writeLength = (writeRemain > block->dataSize()) ? block->dataSize() : writeRemain;

		block->write(ptr, writeLength);
		ptr += writeLength;
		writeRemain -= writeLength;

		block->writeBack(body);
	}

	assert(writeRemain == 0);
}

ArrayList<VariableBlock>* VariableBlockHandle::realloc(uint64_t length) {
	IBlockHandle* handle = this->store->realloc(this->fpos, length); __STP(handle);

	VariableBlockHandle* other = dynamic_cast<VariableBlockHandle*>(handle);
	ByteBuffer* b = other->moveBuffer();

	setBuffer(b);

	// update handle
	this->fpos = handle->getFpos();

	return this->store->getBlockList(this->fpos);
}

bool VariableBlockHandle::needRealloc(ArrayList<VariableBlock>* list, uint64_t length) {
	uint64_t blockUnitSize = this->store->getHeader()->getBlockUnitSize();

	uint64_t availbleSize = getAvailableSize(list);
	uint64_t leftUnused = availbleSize > length ? availbleSize - length : 0;
	return (length > availbleSize) || (leftUnused >= blockUnitSize);
}

uint64_t VariableBlockHandle::getAvailableSize(ArrayList<VariableBlock> *list) {
	uint64_t ret = 0;

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		VariableBlock* block = list->get(i);

		uint16_t available = block->dataSize();
		ret += available;
	}

	return ret;
}

ByteBuffer* VariableBlockHandle::getBuffer() const {
	return this->buffer;
}

void VariableBlockHandle::removeBlocks() {
	ArrayList<VariableBlock>* list = this->store->getBlockList(this->fpos); __STP(list);
	list->setDeleteOnExit();

	removeBlocks(list);
}

void VariableBlockHandle::removeBlocks(ArrayList<VariableBlock> *list) {
	VariableBlockHeader* header = this->store->getHeader();
	VariableBlockFileBody* body = this->store->getBody();

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		VariableBlock* block = list->get(i);

		block->freeBlock(header, body);
	}
	this->store->sync(false);
}

int VariableBlockHandle::size() const noexcept {
	return this->buffer->limit();
}

void VariableBlockHandle::setBuffer(ByteBuffer *buffer) {
	delete this->buffer;
	this->buffer = buffer;
}

ByteBuffer* VariableBlockHandle::moveBuffer() noexcept {
	ByteBuffer* ret = this->buffer;
	this->buffer = nullptr;

	return ret;
}

} /* namespace alinous */
