/*
 * StakingSeed.cpp
 *
 *  Created on: Jul 28, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net/StakingEncriptedSeed.h"

#include "base_io/ByteBuffer.h"


namespace codablecash {

StakingEncriptedSeed::StakingEncriptedSeed(const uint8_t* binary, int length) {
	this->encryptedSeed = ByteBuffer::wrapWithEndian((const uint8_t*)binary, length, true);
}

StakingEncriptedSeed::~StakingEncriptedSeed() {
	delete this->encryptedSeed;
}

} /* namespace codablecash */
