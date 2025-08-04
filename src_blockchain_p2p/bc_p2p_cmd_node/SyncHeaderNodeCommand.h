/*
 * SyncHeaderNodeCommand.h
 *
 *  Created on: 2023/11/06
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_SYNCHEADERNODECOMMAND_H_
#define BC_P2P_CMD_NODE_SYNCHEADERNODECOMMAND_H_

#include "bc_p2p_cmd_node/AbstractNodeCommand.h"


namespace codablecash {

class SyncHeaderNodeCommand : public AbstractNodeCommand {
public:
	SyncHeaderNodeCommand(const SyncHeaderNodeCommand& inst);
	SyncHeaderNodeCommand();
	virtual ~SyncHeaderNodeCommand();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual void fromBinary(ByteBuffer* buff);

	virtual ByteBuffer* getSignBinary() const;
	virtual IBlockObject* copyData() const noexcept;

	virtual AbstractCommandResponse* executeAsNode(BlockchainNodeHandshake* nodeHandShake, CodablecashNodeInstance* inst, bool suspend) const;

	void setZone(uint16_t zone) noexcept;
	void setNumRequestHeight(uint32_t numRequestHeight) noexcept;
	void setStartHeight(uint32_t offset) noexcept;

	virtual bool usePendingQueue() const noexcept;

private:
	uint16_t zone;
	uint32_t numRequestHeight;
	uint64_t startHeight;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_SYNCHEADERNODECOMMAND_H_ */
