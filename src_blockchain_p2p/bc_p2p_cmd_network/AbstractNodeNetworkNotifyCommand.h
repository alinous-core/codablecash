/*
 * AbstractNodeNetworkNotifyCommand.h
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NETWORK_ABSTRACTNODENETWORKNOTIFYCOMMAND_H_
#define BC_P2P_CMD_NETWORK_ABSTRACTNODENETWORKNOTIFYCOMMAND_H_

#include "bc_p2p_cmd_node/AbstractNodeCommand.h"

namespace codablecash {

class BlockchainNodeHandshake;
class CodablecashNodeInstance;

class AbstractNodeNetworkNotifyCommand : public AbstractNodeCommand {
public:
	AbstractNodeNetworkNotifyCommand(const AbstractNodeNetworkNotifyCommand& inst);
	explicit AbstractNodeNetworkNotifyCommand(uint32_t type);
	virtual ~AbstractNodeNetworkNotifyCommand();

	virtual bool usePendingQueue() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NETWORK_ABSTRACTNODENETWORKNOTIFYCOMMAND_H_ */
