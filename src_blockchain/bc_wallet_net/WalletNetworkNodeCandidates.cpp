/*
 * NetworkNodeCandidates.cpp
 *
 *  Created on: Mar 14, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net/WalletNetworkNodeCandidates.h"

#include "bc_p2p_info/P2pNodeRecord.h"

#include "bc_network/NodeIdentifier.h"

#include "base/StackRelease.h"


namespace codablecash {

WalletNetworkNodeCandidates::WalletNetworkNodeCandidates() {
	this->list = new ArrayList<P2pNodeRecord>();

}

WalletNetworkNodeCandidates::~WalletNetworkNodeCandidates() {
	this->list->deleteElements();
	delete this->list;
}

void WalletNetworkNodeCandidates::addRecord(const P2pNodeRecord *rec) {
	int index = getIndexOf(rec);
	if(index >= 0){
		delete this->list->remove(index);
	}

	this->list->addElement(new P2pNodeRecord(*rec));
}

int WalletNetworkNodeCandidates::size() const noexcept {
	return this->list->size();
}

int WalletNetworkNodeCandidates::getIndexOf(const P2pNodeRecord *rec) noexcept {
	int ret = -1;

	const NodeIdentifier* nodeId = rec->getNodeIdentifier();

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		P2pNodeRecord* r = this->list->get(i);

		const NodeIdentifier* id = r->getNodeIdentifier();
		if(nodeId->equals(id)){
			ret = i;
			break;
		}
	}

	return ret;
}

ArrayList<const P2pNodeRecord>* WalletNetworkNodeCandidates::getCandidatesList() const noexcept {
	ArrayList<const P2pNodeRecord>* list = new ArrayList<const P2pNodeRecord>(); __STP(list);


	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		P2pNodeRecord* r = this->list->get(i);

		list->addElement(r);
	}

	return __STP_MV(list);
}

} /* namespace codablecash */
