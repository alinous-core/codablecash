/*
 * PingNodeCommand.h
 *
 *  Created on: 2023/09/10
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_PINGNODECOMMAND_H_
#define BC_P2P_CMD_CLIENT_PINGNODECOMMAND_H_

#include "bc_p2p_cmd_client/AbstractClientRequestCommand.h"

namespace codablecash {

class PingNodeCommand : public AbstractClientRequestCommand {
public:
	PingNodeCommand(const PingNodeCommand& inst);
	PingNodeCommand();
	virtual ~PingNodeCommand();

	virtual IBlockObject* copyData() const noexcept;

protected:
	virtual AbstractCommandResponse* executeAsClient(ClientNodeHandshake* clientHandshake, CodablecashNodeInstance* inst) const;

	virtual ByteBuffer* getSignBinary() const;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_PINGNODECOMMAND_H_ */
