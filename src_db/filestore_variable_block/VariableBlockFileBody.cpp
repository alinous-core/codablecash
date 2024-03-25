/*
 * VariableBlockFileBody.cpp
 *
 *  Created on: 2023/02/08
 *      Author: iizuka
 */

#include "filestore_variable_block/VariableBlockFileBody.h"
#include "filestore_variable_block/VariableBlock.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"

#include "random_access_file/RandomAccessFile.h"

#include "filestore_block/exceptions.h"


namespace alinous {

VariableBlockFileBody::VariableBlockFileBody(RandomAccessFile* file, uint64_t blockUnitSize) {
	this->file = file;
	this->blockUnitSize = blockUnitSize;
}

VariableBlockFileBody::~VariableBlockFileBody() {
	this->file = nullptr;
}

void VariableBlockFileBody::createStore(bool del, uint64_t blockUnitSize) {
	this->blockUnitSize = blockUnitSize;
}

void VariableBlockFileBody::sync(bool fileSync) {
	this->file->sync(fileSync);
}

void VariableBlockFileBody::resetHeader(uint64_t fpos) {
	static const char tmp[VariableBlock::HEADER_SIZE]{};

	this->file->write(fpos, tmp, VariableBlock::HEADER_SIZE);
}

void VariableBlockFileBody::extend(uint64_t newLength) {
	this->file->setLength(newLength);
}

} /* namespace alinous */
