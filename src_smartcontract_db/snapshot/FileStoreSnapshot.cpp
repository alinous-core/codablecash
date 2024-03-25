/*
 * FileStoreSnapshot.cpp
 *
 *  Created on: 2023/06/30
 *      Author: iizuka
 */

#include "snapshot/FileStoreSnapshot.h"

#include "base_io/File.h"
#include "base_io/ByteBuffer.h"

#include "base_io_stream/FileOutputStream.h"

#include "snapshot/AbstractSnapshotCommand.h"

#include "base/StackRelease.h"

#include "bc_base/BinaryUtils.h"

namespace codablecash {

FileStoreSnapshot::FileStoreSnapshot() {
	this->outStream = nullptr;
}

FileStoreSnapshot::~FileStoreSnapshot() {
	close();
}

void FileStoreSnapshot::openOutFile(const File *file) {
	file->deleteFile();
	this->outStream = new FileOutputStream(file, true);
	this->outStream->open(false);
}

void FileStoreSnapshot::close() noexcept {
	if(this->outStream != nullptr){
		this->outStream->flush();
		this->outStream->close();
		delete this->outStream, this->outStream = nullptr;
	}
}

void FileStoreSnapshot::putCommand(const AbstractSnapshotCommand *command) {
	int cap = command->binarySize();

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(cap, true); __STP(buff);

	command->toBinary(buff);
	__ASSERT_POS(buff);

	buff->position(0);

	ByteBuffer* sizebuff =ByteBuffer::allocateWithEndian(sizeof(uint16_t), true); __STP(sizebuff);
	sizebuff->putShort(cap);
	sizebuff->position(0);

	this->outStream->write((const char*)sizebuff->array(), sizebuff->limit());
	this->outStream->write((const char*)buff->array(), buff->limit());
}

} /* namespace codablecash */
