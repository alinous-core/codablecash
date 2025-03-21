/*
 * NetworkNodeCandidates.h
 *
 *  Created on: Mar 14, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_WALLETNETWORKNODECANDIDATES_H_
#define BC_WALLET_NET_WALLETNETWORKNODECANDIDATES_H_


#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class P2pNodeRecord;

class WalletNetworkNodeCandidates {
public:
	WalletNetworkNodeCandidates();
	virtual ~WalletNetworkNodeCandidates();

	void addRecord(const P2pNodeRecord* rec);

	int size() const noexcept;
	int getIndexOf(const P2pNodeRecord* rec) noexcept;

	ArrayList<const P2pNodeRecord>* getCandidatesList() const noexcept;

private:
	ArrayList<P2pNodeRecord>* list;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_WALLETNETWORKNODECANDIDATES_H_ */
