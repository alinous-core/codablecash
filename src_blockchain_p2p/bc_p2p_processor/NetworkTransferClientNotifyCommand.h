/*
 * NetworkTransferClientNotifyCommand.h
 *
 *  Created on: 2023/10/07
 *      Author: iizuka
 */

#ifndef BC_P2P_PROCESSOR_NETWORKTRANSFERCLIENTNOTIFYCOMMAND_H_
#define BC_P2P_PROCESSOR_NETWORKTRANSFERCLIENTNOTIFYCOMMAND_H_

#include "bc_p2p_processor/AbstractNetworkTransferCommandMessage.h"

namespace codablecash {

class NodeIdentifier;
class AbstractClientNotifyCommand;

class NetworkTransferClientNotifyCommand : public AbstractNetworkTransferCommandMessage {
public:
	NetworkTransferClientNotifyCommand(const NodeIdentifier *nodeId, const AbstractClientNotifyCommand *command);
	virtual ~NetworkTransferClientNotifyCommand();

	virtual void execute(ICommandParameter* param);

private:
	NodeIdentifier *nodeId; // destination
	AbstractClientNotifyCommand *command;
};

} /* namespace codablecash */

#endif /* BC_P2P_PROCESSOR_NETWORKTRANSFERCLIENTNOTIFYCOMMAND_H_ */
