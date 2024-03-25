/*
 * StackHandshakeReleaser.cpp
 *
 *  Created on: 2023/10/02
 *      Author: iizuka
 */

#include "bc_p2p/StackHandshakeReleaser.h"
#include "bc_p2p/BlockchainNodeHandshake.h"


namespace codablecash {

StackHandshakeReleaser::StackHandshakeReleaser(BlockchainNodeHandshake* handshake) {
	this->handshake = handshake;
}

StackHandshakeReleaser::~StackHandshakeReleaser() {
	if(this->handshake != nullptr){
		this->handshake->decRef();
	}
}

} /* namespace codablecash */
