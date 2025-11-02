/*
 * ListStakingNodeAccess.h
 *
 *  Created on: Aug 10, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_ACCESS_LISTSTAKINGNODEACCESS_H_
#define BC_WALLET_NET_ACCESS_LISTSTAKINGNODEACCESS_H_

#include "bc_wallet_net_access/IRetriableClientAccess.h"

namespace codablecash {

class WalletNetworkManager;
class ClientListStakingNodeIdsCommand;
class ClientListStakingNodeIdsCommandResponse;

class ListStakingNodeAccess : public IRetriableClientAccess {
public:
	explicit ListStakingNodeAccess(const ClientListStakingNodeIdsCommand* command);
	virtual ~ListStakingNodeAccess();

	virtual void access(ClientNodeHandshake* handshake, ISystemLogger* logger);


	const ClientListStakingNodeIdsCommandResponse* getResponse() const noexcept {
		return this->response;
	}

private:
	const ClientListStakingNodeIdsCommand* command;
	ClientListStakingNodeIdsCommandResponse* response;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_ACCESS_LISTSTAKINGNODEACCESS_H_ */
