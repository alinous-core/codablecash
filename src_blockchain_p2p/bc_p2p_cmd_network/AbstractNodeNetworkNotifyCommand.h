/*
 * AbstractNodeNetworkNotifyCommand.h
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NETWORK_ABSTRACTNODENETWORKNOTIFYCOMMAND_H_
#define BC_P2P_CMD_NETWORK_ABSTRACTNODENETWORKNOTIFYCOMMAND_H_

#include "bc_p2p_cmd/AbstractBlockchainPubsubCommand.h"

namespace codablecash {

class AbstractNodeNetworkNotifyCommand : public AbstractBlockchainPubsubCommand {
public:
	explicit AbstractNodeNetworkNotifyCommand(uint32_t type);
	virtual ~AbstractNodeNetworkNotifyCommand();
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NETWORK_ABSTRACTNODENETWORKNOTIFYCOMMAND_H_ */
