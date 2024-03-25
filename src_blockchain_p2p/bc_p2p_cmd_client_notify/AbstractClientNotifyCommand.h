/*
 * AbstractClientNotify.h
 *
 *  Created on: 2023/10/05
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_NOTIFY_ABSTRACTCLIENTNOTIFYCOMMAND_H_
#define BC_P2P_CMD_CLIENT_NOTIFY_ABSTRACTCLIENTNOTIFYCOMMAND_H_

#include "bc_p2p_cmd/AbstractBlockchainPubsubCommand.h"

namespace codablecash {

class ClientExecutor;

class AbstractClientNotifyCommand : public AbstractBlockchainPubsubCommand {
public:
	AbstractClientNotifyCommand(const AbstractClientNotifyCommand& info);
	explicit AbstractClientNotifyCommand(uint32_t type);
	virtual ~AbstractClientNotifyCommand();

	virtual AbstractCommandResponse* execute(const PubSubId *pubsubId, IPubsubCommandListner *listner) const;

protected:
	virtual AbstractCommandResponse* executeNotify(const PubSubId *pubsubId, ClientExecutor* clientExec) const = 0;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_NOTIFY_ABSTRACTCLIENTNOTIFYCOMMAND_H_ */
