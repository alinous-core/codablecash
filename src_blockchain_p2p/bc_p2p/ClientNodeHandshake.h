/*
 * ClientNodeHandshake.h
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#ifndef BC_P2P_CLIENTNODEHANDSHAKE_H_
#define BC_P2P_CLIENTNODEHANDSHAKE_H_

#include "bc_p2p/BlockchainNodeHandshake.h"

namespace codablecash {

class ClientNodeHandshake : public BlockchainNodeHandshake {
public:
	explicit ClientNodeHandshake(P2pHandshake *handshake, int zone, const NodeIdentifier* nodeId);
	virtual ~ClientNodeHandshake();
};

} /* namespace codablecash */

#endif /* BC_P2P_CLIENTNODEHANDSHAKE_H_ */
