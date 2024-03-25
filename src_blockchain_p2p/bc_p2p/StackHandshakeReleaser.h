/*
 * StackHandshakeReleaser.h
 *
 *  Created on: 2023/10/02
 *      Author: iizuka
 */

#ifndef BC_P2P_STACKHANDSHAKERELEASER_H_
#define BC_P2P_STACKHANDSHAKERELEASER_H_

namespace codablecash {

class BlockchainNodeHandshake;

class StackHandshakeReleaser {
public:
	explicit StackHandshakeReleaser(BlockchainNodeHandshake* handshake);
	virtual ~StackHandshakeReleaser();

private:
	BlockchainNodeHandshake* handshake;
};

} /* namespace codablecash */

#endif /* BC_P2P_STACKHANDSHAKERELEASER_H_ */
