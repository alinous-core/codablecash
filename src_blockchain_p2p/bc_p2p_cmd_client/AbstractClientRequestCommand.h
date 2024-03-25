/*
 * AbstractClientRequestCommand.h
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_ABSTRACTCLIENTREQUESTCOMMAND_H_
#define BC_P2P_CMD_CLIENT_ABSTRACTCLIENTREQUESTCOMMAND_H_

#include "bc_p2p_cmd/AbstractBlockchainPubsubCommand.h"

namespace codablecash {

class ClientNodeHandshake;
class CodablecashNodeInstance;

class AbstractClientRequestCommand : public AbstractBlockchainPubsubCommand {
public:
	AbstractClientRequestCommand(const AbstractClientRequestCommand& inst);
	explicit AbstractClientRequestCommand(uint32_t type);
	virtual ~AbstractClientRequestCommand();

	virtual AbstractCommandResponse* execute(const PubSubId *pubsubId, IPubsubCommandListner *listner) const;

protected:
	virtual AbstractCommandResponse* executeAsClient(ClientNodeHandshake* clientHandshake, CodablecashNodeInstance* inst) const = 0;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_ABSTRACTCLIENTREQUESTCOMMAND_H_ */
