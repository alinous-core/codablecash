/*
 * ReportMinedBlockNodeCommand.h
 *
 *  Created on: 2023/11/21
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_REPORTMINEDBLOCKNODECOMMAND_H_
#define BC_P2P_CMD_NODE_REPORTMINEDBLOCKNODECOMMAND_H_

#include "bc_p2p_cmd_node/AbstractNodeCommand.h"

namespace codablecash {

class BlockHeaderTransferData;

class ReportMinedBlockNodeCommand : public AbstractNodeCommand {
public:
	ReportMinedBlockNodeCommand(const ReportMinedBlockNodeCommand& inst);
	ReportMinedBlockNodeCommand();
	virtual ~ReportMinedBlockNodeCommand();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual void fromBinary(ByteBuffer* buff);

	virtual ByteBuffer* getSignBinary() const;
	virtual IBlockObject* copyData() const noexcept;

	virtual AbstractCommandResponse* executeAsNode(BlockchainNodeHandshake* nodeHandShake, CodablecashNodeInstance* inst, bool suspend) const;

	void setData(const BlockHeaderTransferData* data);

private:
	BlockHeaderTransferData* data;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_REPORTMINEDBLOCKNODECOMMAND_H_ */
