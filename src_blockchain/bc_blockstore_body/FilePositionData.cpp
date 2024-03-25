/*
 * FilePositionData.cpp
 *
 *  Created on: 2023/03/02
 *      Author: iizuka
 */

#include "bc_blockstore_body/FilePositionData.h"

namespace codablecash {

FilePositionData::FilePositionData(uint64_t fpos) {
	this->fpos = fpos;
}

FilePositionData::~FilePositionData() {

}

int FilePositionData::binarySize() const {
	return sizeof(this->fpos);
}

void FilePositionData::toBinary(ByteBuffer *out) const {
	out->putLong(this->fpos);
}

FilePositionData* FilePositionData::createFromBinary(ByteBuffer *in) {
	uint64_t fpos = in->getLong();
	return new FilePositionData(fpos);
}

IBlockObject* FilePositionData::copyData() const noexcept {
	return new FilePositionData(this->fpos);
}

} /* namespace codablecash */
