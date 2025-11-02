/*
 * ClientNodeHandshake.cpp
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#include "bc_p2p/ClientNodeHandshake.h"

#include "base_thread/StackUnlocker.h"
#include "base_thread/SysMutex.h"
#include "bc_wallet_filter/BloomFilter1024.h"

namespace codablecash {

ClientNodeHandshake::ClientNodeHandshake(P2pHandshake *handshake, int zone, const NodeIdentifier* nodeId) : BlockchainNodeHandshake(handshake, zone, nodeId, nullptr) {
	this->filterList = new ArrayList<BloomFilter1024>();
}

ClientNodeHandshake::~ClientNodeHandshake() {
	this->filterList->deleteElements();
	delete this->filterList;
}

void ClientNodeHandshake::addBloomFilter(const BloomFilter1024 *f) {
	BloomFilter1024* filter = dynamic_cast<BloomFilter1024*>(f->copyData());
	this->filterList->addElement(filter);
}

} /* namespace codablecash */
