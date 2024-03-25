/*
 * FilePositionDataFactory.cpp
 *
 *  Created on: 2023/03/02
 *      Author: iizuka
 */

#include "bc_blockstore_body/FilePositionDataFactory.h"
#include "bc_blockstore_body/FilePositionData.h"

namespace codablecash {

FilePositionDataFactory::FilePositionDataFactory() {

}

FilePositionDataFactory::~FilePositionDataFactory() {

}

IBlockObject* FilePositionDataFactory::makeDataFromBinary(ByteBuffer *in) {
	return FilePositionData::createFromBinary(in);
}

AbstractBtreeDataFactory* FilePositionDataFactory::copy() const noexcept {
	return new FilePositionDataFactory();
}

} /* namespace codablecash */
