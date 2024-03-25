/*
 * SnapShotSha256.cpp
 *
 *  Created on: 2023/07/03
 *      Author: iizuka
 */

#include "snapshot/SnapShotSha256.h"

#include "base_io/File.h"

#include "base_io_stream/FileInputStream.h"

#include "crypto/Sha256Context.h"

#include "base/StackRelease.h"

#include <cstdint>

using alinous::Mem;

#include "osenv/memory.h"
namespace codablecash {

SnapShotSha256::SnapShotSha256(const File* file) {
	this->file = new File(*file);
	this->context = nullptr;
}

SnapShotSha256::~SnapShotSha256() {
	delete this->file;
	delete this->context;
}

void SnapShotSha256::process() {
	delete this->context;
	this->context = new Sha256Context();

	FileInputStream* stream = new FileInputStream(this->file); __STP(stream);
	stream->open();

	char data[256];

	int n = stream->read(data, 256);
	while(n > 0){
		this->context->update(data, n);

		n = stream->read(data, 256);
	}

	stream->close();

	this->context->finalize();
}

bool SnapShotSha256::equals(const SnapShotSha256 *other) const noexcept {
	const uint8_t* data = this->context->getData32();
	const uint8_t* data2 = other->context->getData32();

	int cmp = Mem::memcmp(data, data2, 32);
	return cmp == 0;
}

} /* namespace codablecash */
