/*
 * VariableBlock.cpp
 *
 *  Created on: 2023/02/08
 *      Author: iizuka
 */

#include "filestore_variable_block/VariableBlock.h"
#include "filestore_variable_block/VariableBlockFileBody.h"
#include "filestore_variable_block/VariableBlockHeader.h"

#include "osenv/memory.h"

#include "random_access_file/RandomAccessFile.h"

#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"

#include "filestore/LongRange.h"

namespace alinous {

VariableBlock::VariableBlock(uint16_t blockSize, uint64_t fpos, uint16_t used, uint64_t nextfpos, const char* data) {
	this->blockSize = blockSize;
	this->currentfPos = fpos;
	this->used = used;
	this->nextfpos = nextfpos;

	this->data = new char[dataSize()]{};
	if(data != nullptr){
		Mem::memcpy(this->data, data, used);
	}

}

VariableBlock::~VariableBlock() {
	delete [] this->data;
}

void VariableBlock::writeBack(VariableBlockFileBody *body) {
	RandomAccessFile* rfile = body->getFile();

	uint64_t fpos = this->currentfPos;
	{
		ByteBuffer* buff = ByteBuffer::allocateWithEndian(HEADER_SIZE, true); __STP(buff);
		buff->putShort(this->used);
		buff->putLong(this->nextfpos);

		buff->position(0);
		const char* d = (const char*)buff->array();
		fpos += rfile->write(fpos, d, buff->limit());
	}

	fpos += rfile->write(fpos, (const char*)this->data, dataSize());

	assert((fpos - this->currentfPos) == this->blockSize);
}

void VariableBlock::write(const char *buff, int length) {
	Mem::memcpy(this->data, buff, length);
	this->used = length;
}

VariableBlock* VariableBlock::load(VariableBlockFileBody *body, uint64_t fpos, uint16_t blockunit) {
	RandomAccessFile* rfile = body->getFile();

	uint64_t __fpos = fpos;

	char tmp[HEADER_SIZE];
	__fpos += rfile->read(__fpos, tmp, HEADER_SIZE);

	ByteBuffer* buff = ByteBuffer::wrapWithEndian(tmp, HEADER_SIZE, true); __STP(buff);
	uint16_t used = buff->getShort();
	uint64_t nextfpos = buff->getLong();
	uint16_t blockSize = toBlockSize(used, blockunit);

	char* data = new char[used];
	StackArrayRelease<char> __st_data(data);

	__fpos += rfile->read(__fpos, data, used);

	VariableBlock* block = new VariableBlock(blockSize, fpos, used, nextfpos, data);
	return block;
}

uint16_t VariableBlock::toBlockSize(uint16_t used, uint16_t blockunit) noexcept {
	uint16_t total = used + HEADER_SIZE;
	uint16_t mod = total % blockunit;
	uint16_t ret = total - mod;
	if(mod != 0){
		ret += blockunit;
	}

	return ret;
}

void VariableBlock::freeBlock(VariableBlockHeader *header,	VariableBlockFileBody *body) {
	uint64_t blockUnitSize = header->getBlockUnitSize();

	LongRange* range = getLongRange(blockUnitSize); __STP(range);
	header->freeFragment(range);

	body->resetHeader(this->currentfPos);
}

LongRange* VariableBlock::getLongRange(uint64_t blockUnitSize) const noexcept {
	uint64_t min = this->currentfPos / blockUnitSize;
	int numBlocks = this->blockSize / blockUnitSize;

	assert(this->currentfPos % blockUnitSize == 0);
	assert(this->blockSize % blockUnitSize == 0);

	uint64_t max = min + numBlocks - 1;

	return new LongRange(min, max);
}

} /* namespace alinous */
