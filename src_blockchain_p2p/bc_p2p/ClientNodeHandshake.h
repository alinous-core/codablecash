/*
 * ClientNodeHandshake.h
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#ifndef BC_P2P_CLIENTNODEHANDSHAKE_H_
#define BC_P2P_CLIENTNODEHANDSHAKE_H_

#include "bc_p2p/BlockchainNodeHandshake.h"

#include "base/ArrayList.h"

namespace codablecash {
class BloomFilter512;

class ClientNodeHandshake : public BlockchainNodeHandshake {
public:
	explicit ClientNodeHandshake(P2pHandshake *handshake, int zone, const NodeIdentifier* nodeId);
	virtual ~ClientNodeHandshake();

	void addBloomFilter(const BloomFilter512* f);

private:
	ArrayList<BloomFilter512>* filterList;
};

} /* namespace codablecash */

#endif /* BC_P2P_CLIENTNODEHANDSHAKE_H_ */
