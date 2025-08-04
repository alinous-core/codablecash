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
class BloomFilter1024;

class ClientNodeHandshake : public BlockchainNodeHandshake {
public:
	explicit ClientNodeHandshake(P2pHandshake *handshake, int zone, const NodeIdentifier* nodeId);
	virtual ~ClientNodeHandshake();

	void addBloomFilter(const BloomFilter1024* f);
	const ArrayList<BloomFilter1024>* getBloomFilters() const noexcept {
		return this->filterList;
	}

private:
	ArrayList<BloomFilter1024>* filterList;
};

} /* namespace codablecash */

#endif /* BC_P2P_CLIENTNODEHANDSHAKE_H_ */
