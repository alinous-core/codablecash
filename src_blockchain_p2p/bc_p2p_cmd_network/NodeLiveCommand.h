/*
 * NodeLiveCommand.h
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NETWORK_NODELIVECOMMAND_H_
#define BC_P2P_CMD_NETWORK_NODELIVECOMMAND_H_

#include "bc_p2p_cmd_network/AbstractNodeNetworkNotifyCommand.h"

namespace codablecash {

class NodeLiveData;

class NodeLiveCommand : public AbstractNodeNetworkNotifyCommand {
public:
	NodeLiveCommand();
	virtual ~NodeLiveCommand();

private:
	NodeLiveData* data;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NETWORK_NODELIVECOMMAND_H_ */
