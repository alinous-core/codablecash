/*
 * NopNodeCommand.h
 *
 *  Created on: 2023/12/04
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_NOPNODECOMMAND_H_
#define BC_P2P_CMD_NODE_NOPNODECOMMAND_H_

#include "bc_p2p_cmd_node/AbstractNodeCommand.h"

namespace codablecash {

class NopNodeCommand: public AbstractNodeCommand {
public:
	NopNodeCommand(const NopNodeCommand& inst);
	NopNodeCommand();
	virtual ~NopNodeCommand();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual void fromBinary(ByteBuffer* buff);

	virtual ByteBuffer* getSignBinary() const;
	virtual IBlockObject* copyData() const noexcept;

	virtual AbstractCommandResponse* executeAsNode(BlockchainNodeHandshake* nodeHandShake, CodablecashNodeInstance* inst, bool suspend) const;

	void setValue(uint8_t v) noexcept;

private:
	uint8_t value;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_NOPNODECOMMAND_H_ */
