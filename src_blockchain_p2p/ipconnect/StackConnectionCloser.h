/*
 * StackConnectionCloser.h
 *
 *  Created on: Feb 2, 2025
 *      Author: iizuka
 */

#ifndef IPCONNECT_STACKCONNECTIONCLOSER_H_
#define IPCONNECT_STACKCONNECTIONCLOSER_H_

namespace codablecash {

class IClientSocket;

class StackConnectionCloser {
public:
	explicit StackConnectionCloser(IClientSocket* con);
	virtual ~StackConnectionCloser();

private:
	IClientSocket* con;
};

} /* namespace codablecash */

#endif /* IPCONNECT_STACKCONNECTIONCLOSER_H_ */
