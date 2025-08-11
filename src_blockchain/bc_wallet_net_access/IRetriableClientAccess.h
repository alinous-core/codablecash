/*
 * IRetriable.h
 *
 *  Created on: Aug 10, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_ACCESS_IRETRIABLECLIENTACCESS_H_
#define BC_WALLET_NET_ACCESS_IRETRIABLECLIENTACCESS_H_

namespace codablecash {

class ClientNodeHandshake;
class ISystemLogger;

class IRetriableClientAccess {
public:
	IRetriableClientAccess();
	virtual ~IRetriableClientAccess();

	virtual void access(ClientNodeHandshake* handshake, ISystemLogger* logger) = 0;


};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_ACCESS_IRETRIABLECLIENTACCESS_H_ */
