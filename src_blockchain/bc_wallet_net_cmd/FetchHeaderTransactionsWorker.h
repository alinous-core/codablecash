/*
 * FetchHeaderTransactionsWorker.h
 *
 *  Created on: Aug 20, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_CMD_FETCHHEADERTRANSACTIONSWORKER_H_
#define BC_WALLET_NET_CMD_FETCHHEADERTRANSACTIONSWORKER_H_

#include "bc_network_instance_sync/AbstractSyncWorkerThread.h"
#include <cstdint>

namespace codablecash {

class BlockHeaderId;
class NodeIdentifier;
class WalletNetworkManager;
class ClientFetchHeaderTransactionsCommandResponse;

class FetchHeaderTransactionsWorker : public AbstractSyncWorkerThread {
public:
	FetchHeaderTransactionsWorker(uint16_t zone, const BlockHeaderId* headerId, uint64_t height, const NodeIdentifier* nodeId,
			WalletNetworkManager* networkManager, ISystemLogger* logger, const UnicodeString* name);
	virtual ~FetchHeaderTransactionsWorker();

	const ClientFetchHeaderTransactionsCommandResponse* getResponse() const noexcept {
		return this->response;
	}

protected:
	virtual void doProcess();

private:
	uint16_t zone;
	BlockHeaderId* headerId;
	uint64_t height;

	NodeIdentifier* nodeId;
	WalletNetworkManager* networkManager;

	ClientFetchHeaderTransactionsCommandResponse* response;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_CMD_FETCHHEADERTRANSACTIONSWORKER_H_ */
