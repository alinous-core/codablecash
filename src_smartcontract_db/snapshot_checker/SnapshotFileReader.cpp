/*
 * SnapshotFileReader.cpp
 *
 *  Created on: 2023/07/29
 *      Author: iizuka
 */

#include "snapshot_checker/SnapshotFileReader.h"

#include "snapshot/AbstractSnapshotCommand.h"

#include "base_io/File.h"
#include "base_io/ByteBuffer.h"

#include "base_io_stream/FileInputStream.h"

#include "base/StackRelease.h"


namespace codablecash {

SnapshotFileReader::SnapshotFileReader(const File* file) {
	this->file = new File(*file);
	this->inStream = nullptr;
}

SnapshotFileReader::~SnapshotFileReader() {
	close();
	delete this->file;
}

void SnapshotFileReader::close() noexcept {
	if(this->inStream != nullptr){
		this->inStream->close();
		delete this->inStream, this->inStream = nullptr;
	}
}

void SnapshotFileReader::init() {
	this->inStream = new FileInputStream(file);
	this->inStream->open();
}

uint16_t SnapshotFileReader::readShort() {
	char tmp[2];
	int read = this->inStream->read(tmp, 2);
	if(read <= 0){
		return 0;
	}

	ByteBuffer* buff = ByteBuffer::wrapWithEndian(tmp, 2, true); __STP(buff);
	buff->position(0);

	return buff->getShort();
}

AbstractSnapshotCommand* SnapshotFileReader::readCommand(uint16_t size) {
	char* data = new char[size];
	StackArrayRelease<char> __data(data);

	int n = this->inStream->read(data, size);

	ByteBuffer* buff = ByteBuffer::wrapWithEndian(data, n, true); __STP(buff);

	return AbstractSnapshotCommand::createFromBinary(buff);
}

} /* namespace codablecash */
