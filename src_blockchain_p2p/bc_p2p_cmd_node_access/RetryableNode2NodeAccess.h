/*
 * RetryableNode2NodeAccess.h
 *
 *  Created on: 2023/11/19
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_RETRYABLENODE2NODEACCESS_H_
#define BC_P2P_CMD_NODE_RETRYABLENODE2NODEACCESS_H_

#include <cstdint>

namespace codablecash {

class AbstractNodeCommand;
class BlochchainP2pManager;
class AbstractCommandResponse;
class IRetryableResponceValidator;
class ISystemLogger;

class RetryableNode2NodeAccess {
public:
	RetryableNode2NodeAccess(uint16_t zone, AbstractNodeCommand* command, int retryTimes, BlochchainP2pManager* p2pManager
			, IRetryableResponceValidator* validator, ISystemLogger* logger);
	virtual ~RetryableNode2NodeAccess();

	AbstractCommandResponse* access();

private:
	uint16_t zone;
	int retryTimes;
	AbstractNodeCommand* command;
	BlochchainP2pManager* p2pManager;

	IRetryableResponceValidator* validator;
	ISystemLogger* logger;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_RETRYABLENODE2NODEACCESS_H_ */
