/*
 * DownloadBlockNodeCommand.h
 *
 *  Created on: 2023/11/18
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_DOWNLOADBLOCKNODECOMMAND_H_
#define BC_P2P_CMD_NODE_DOWNLOADBLOCKNODECOMMAND_H_

#include "bc_p2p_cmd_node/AbstractNodeCommand.h"

namespace codablecash {

class BlockHeaderId;

class DownloadBlockNodeCommand : public AbstractNodeCommand {
public:
	DownloadBlockNodeCommand(const DownloadBlockNodeCommand& inst);
	DownloadBlockNodeCommand();
	virtual ~DownloadBlockNodeCommand();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual void fromBinary(ByteBuffer* buff);

	virtual ByteBuffer* getSignBinary() const;
	virtual IBlockObject* copyData() const noexcept;

	virtual bool usePendingQueue() const noexcept;

	virtual AbstractCommandResponse* executeAsNode(BlockchainNodeHandshake* nodeHandShake, CodablecashNodeInstance* inst, bool suspend) const;

	void setZone(uint16_t zone) noexcept;
	void setHeight(uint64_t height) noexcept;
	void setHeaderId(const BlockHeaderId* headerId) noexcept;

private:
	uint16_t zone;
	uint64_t height;
	BlockHeaderId* headerId;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_DOWNLOADBLOCKNODECOMMAND_H_ */
