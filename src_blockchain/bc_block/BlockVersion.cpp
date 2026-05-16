/*
 * BlockVersion.cpp
 *
 *  Created on: May 5, 2026
 *      Author: iizuka
 */

#include "bc_block/BlockVersion.h"

#include "bc/SoftwareVersion.h"

#include "base_io/ByteBuffer.h"

namespace codablecash {


BlockVersion::BlockVersion(const BlockVersion &inst) : SoftwareVersion(inst.major, inst.minor, inst.patch) {
}

BlockVersion::BlockVersion(int major, int minor, int patch) : SoftwareVersion(major, minor, patch) {

}

BlockVersion::~BlockVersion() {

}

BlockVersion* BlockVersion::createFromBinary(ByteBuffer *in) {
	uint8_t major = in->get();
	uint8_t minor = in->get();
	uint8_t pathch = in->get();

	return new BlockVersion(major, minor, pathch);
}

} /* namespace codablecash */
