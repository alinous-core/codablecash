/*
 * NetworkTransferCommand.h
 *
 *  Created on: 2023/09/26
 *      Author: iizuka
 */

#ifndef BC_P2P_PROCESSOR_NETWORKTRANSFERNODECOMMAND_H_
#define BC_P2P_PROCESSOR_NETWORKTRANSFERNODECOMMAND_H_

#include "bc_p2p_processor/AbstractNetworkTransferCommandMessage.h"

namespace codablecash {

class NodeIdentifier;
class AbstractNodeCommand;

class NetworkTransferNodeCommand : public AbstractNetworkTransferCommandMessage {
public:
	NetworkTransferNodeCommand(const NodeIdentifier *nodeId, const AbstractNodeCommand *command);
	virtual ~NetworkTransferNodeCommand();

	virtual void execute(ICommandParameter* param);

private:
	NodeIdentifier *nodeId; // destination
	AbstractNodeCommand *command;
};

} /* namespace codablecash */

#endif /* BC_P2P_PROCESSOR_NETWORKTRANSFERNODECOMMAND_H_ */
