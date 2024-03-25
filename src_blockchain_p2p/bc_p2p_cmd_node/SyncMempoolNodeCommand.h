/*
 * DownloadMempoolTransactionNodeCommand.h
 *
 *  Created on: 2023/11/03
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_SYNCMEMPOOLNODECOMMAND_H_
#define BC_P2P_CMD_NODE_SYNCMEMPOOLNODECOMMAND_H_

#include <cstdint>

#include "bc_p2p_cmd_node/AbstractNodeCommand.h"

namespace codablecash {

class SyncMempoolNodeCommand : public AbstractNodeCommand {
public:
	static const constexpr uint8_t TRX_CONTROL{1};
	static const constexpr uint8_t TRX_ICC{2};
	static const constexpr uint8_t TRX_BALANCE{3};
	static const constexpr uint8_t TRX_SMARTCONTRACT{4};


	SyncMempoolNodeCommand(const SyncMempoolNodeCommand& inst);
	SyncMempoolNodeCommand();
	virtual ~SyncMempoolNodeCommand();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual void fromBinary(ByteBuffer* buff);

	virtual ByteBuffer* getSignBinary() const;
	virtual IBlockObject* copyData() const noexcept;

	virtual AbstractCommandResponse* executeAsNode(BlockchainNodeHandshake* nodeHandShake, CodablecashNodeInstance* inst, bool suspend) const;

	void setTrxType(uint8_t trxType) noexcept;
	void setNumRequestTrx(uint32_t numRequestTrx) noexcept;
	void setOffset(uint32_t offset) noexcept;

private:
	uint8_t trxType;
	uint32_t numRequestTrx;
	uint32_t offset;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_SYNCMEMPOOLNODECOMMAND_H_ */
