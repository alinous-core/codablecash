/*
 * ClientFetchMempoolTrxAccess.h
 *
 *  Created on: Aug 17, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_ACCESS_CLIENTFETCHMEMPOOLTRXACCESS_H_
#define BC_WALLET_NET_ACCESS_CLIENTFETCHMEMPOOLTRXACCESS_H_

#include "bc_wallet_net_access/IRetriableClientAccess.h"

namespace codablecash {

class ClientFetchMempoolTrxCommand;
class ClientFetchMempoolTrxCommandResponse;


class ClientFetchMempoolTrxAccess : public IRetriableClientAccess {
public:
	explicit ClientFetchMempoolTrxAccess(const ClientFetchMempoolTrxCommand* command);
	virtual ~ClientFetchMempoolTrxAccess();

	virtual void access(ClientNodeHandshake* handshake, ISystemLogger* logger);

	const ClientFetchMempoolTrxCommandResponse* getResponse() const noexcept {
		return this->response;
	}

private:
	const ClientFetchMempoolTrxCommand* command;
	ClientFetchMempoolTrxCommandResponse* response;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_ACCESS_CLIENTFETCHMEMPOOLTRXACCESS_H_ */
