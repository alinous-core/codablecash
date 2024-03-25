/*
 * NodeShutdownCommand.h
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NETWORK_NODESHUTDOWNCOMMAND_H_
#define BC_P2P_CMD_NETWORK_NODESHUTDOWNCOMMAND_H_

#include "bc_p2p_cmd_network/AbstractNodeNetworkNotifyCommand.h"

namespace codablecash {

class NodeShutdownCommand : public AbstractNodeNetworkNotifyCommand {
public:
	NodeShutdownCommand();
	virtual ~NodeShutdownCommand();
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NETWORK_NODESHUTDOWNCOMMAND_H_ */
