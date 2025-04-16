/*
 * ClientNodeHandshake.cpp
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#include "bc_p2p/ClientNodeHandshake.h"

#include "bc_wallet_filter/BloomFilter512.h"

#include "base_thread/StackUnlocker.h"
#include "base_thread/SysMutex.h"

namespace codablecash {

ClientNodeHandshake::ClientNodeHandshake(P2pHandshake *handshake, int zone, const NodeIdentifier* nodeId) : BlockchainNodeHandshake(handshake, zone, nodeId, nullptr) {
	this->filterList = new ArrayList<BloomFilter512>();
}

ClientNodeHandshake::~ClientNodeHandshake() {
	this->filterList->deleteElements();
	delete this->filterList;
}

void ClientNodeHandshake::addBloomFilter(const BloomFilter512 *f) {
	BloomFilter512* filter = dynamic_cast<BloomFilter512*>(f->copyData());
	this->filterList->addElement(filter);
}

} /* namespace codablecash */
