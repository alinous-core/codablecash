/*
 * BlockchainSoftwareVersion.cpp
 *
 *  Created on: May 5, 2026
 *      Author: iizuka
 */

#include "bc/BlockchainSoftwareVersion.h"

#include "base_io/ByteBuffer.h"

namespace codablecash {
/*
BlockchainSoftwareVersion::BlockchainSoftwareVersion(const BlockchainSoftwareVersion &inst) : SoftwareVersion(inst) {
}*/

BlockchainSoftwareVersion::BlockchainSoftwareVersion(int major, int minor, int patch) : SoftwareVersion(major, minor, patch) {

}

BlockchainSoftwareVersion::~BlockchainSoftwareVersion() {

}
/*
BlockchainSoftwareVersion* BlockchainSoftwareVersion::createFromBinary(ByteBuffer *in) {
	uint8_t major = in->get();
	uint8_t minor = in->get();
	uint8_t pathch = in->get();

	return new BlockchainSoftwareVersion(major, minor, pathch);
}
*/
} /* namespace codablecash */
