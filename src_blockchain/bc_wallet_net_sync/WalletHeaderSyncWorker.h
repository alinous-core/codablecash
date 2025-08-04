/*
 * WalletHeaderSyncWorker.h
 *
 *  Created on: Apr 21, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_SYNC_WALLETHEADERSYNCWORKER_H_
#define BC_WALLET_NET_SYNC_WALLETHEADERSYNCWORKER_H_

#include <cstdint>

#include "bc_network_instance_sync/AbstractSyncWorkerThread.h"


namespace codablecash {

class NodeIdentifier;
class NetworkWalletSync;
class NodeIdentifierSource;
class WalletNetworkManager;


class WalletHeaderSyncWorker : public AbstractSyncWorkerThread {
public:
	WalletHeaderSyncWorker(uint16_t zone, const NodeIdentifier* nodeId, uint64_t startHeight, NetworkWalletSync* parent, ISystemLogger* logger, const UnicodeString* name);
	virtual ~WalletHeaderSyncWorker();

protected:
	virtual void doProcess();

private:
	void syncHeader(WalletNetworkManager *networkManager, const NodeIdentifierSource *source, uint64_t startHeight);

private:
	NodeIdentifier* nodeId;
	NetworkWalletSync* parent;
	uint16_t zone;

	uint64_t startHeight;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_SYNC_WALLETHEADERSYNCWORKER_H_ */
