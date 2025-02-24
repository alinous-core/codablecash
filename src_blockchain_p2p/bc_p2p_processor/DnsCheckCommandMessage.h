/*
 * DnsCheckCommandMessage.h
 *
 *  Created on: Feb 2, 2025
 *      Author: iizuka
 */

#ifndef BC_P2P_PROCESSOR_DNSCHECKCOMMANDMESSAGE_H_
#define BC_P2P_PROCESSOR_DNSCHECKCOMMANDMESSAGE_H_

#include "bc_p2p_processor/AbstractNetworkInfoCommandMessage.h"

namespace codablecash {

class NodeNetworkInfo;
class P2pRequestProcessor;
class ISystemLogger;

class DnsCheckCommandMessage  : public AbstractNetworkInfoCommandMessage {
public:
	DnsCheckCommandMessage();
	virtual ~DnsCheckCommandMessage();

	virtual void execute(ICommandParameter* param);

	void setNodeInfo(const NodeNetworkInfo* info);

private:
	void checkAndRegister(P2pRequestProcessor* processor, ISystemLogger* logger);

private:
	NodeNetworkInfo *nodeNetInfo;
};

} /* namespace codablecash */

#endif /* BC_P2P_PROCESSOR_DNSCHECKCOMMANDMESSAGE_H_ */
