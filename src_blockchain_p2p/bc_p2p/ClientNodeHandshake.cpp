/*
 * ClientNodeHandshake.cpp
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#include "bc_p2p/ClientNodeHandshake.h"

namespace codablecash {

ClientNodeHandshake::ClientNodeHandshake(P2pHandshake *handshake, int zone, const NodeIdentifier* nodeId) : BlockchainNodeHandshake(handshake, zone, nodeId, nullptr) {

}

ClientNodeHandshake::~ClientNodeHandshake() {

}

} /* namespace codablecash */
