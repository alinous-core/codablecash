/*
 * DownloadTransactionsNodeCommand.h
 *
 *  Created on: 2023/11/27
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_DOWNLOADTRANSACTIONSNODECOMMAND_H_
#define BC_P2P_CMD_NODE_DOWNLOADTRANSACTIONSNODECOMMAND_H_

#include <cstdint>

#include "base/ArrayList.h"

#include "bc_p2p_cmd_node/AbstractNodeCommand.h"

using namespace alinous;

namespace codablecash {

class TransactionId;
class AbstractBlockchainTransaction;
class MemoryPool;
class DownloadTransactionEntry;
class BlockHeaderId;
class BlockBody;

class DownloadTransactionsNodeCommand : public AbstractNodeCommand {
public:
	static const constexpr uint8_t TYPE_CONTROL{1};
	static const constexpr uint8_t TYPE_ICC{2};
	static const constexpr uint8_t TYPE_BALANCE{3};
	static const constexpr uint8_t TYPE_SMARTCONTRACT{4};

	DownloadTransactionsNodeCommand(const DownloadTransactionsNodeCommand& inst);
	DownloadTransactionsNodeCommand();
	virtual ~DownloadTransactionsNodeCommand();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual void fromBinary(ByteBuffer* buff);

	virtual ByteBuffer* getSignBinary() const;
	virtual IBlockObject* copyData() const noexcept;

	virtual bool usePendingQueue() const noexcept;

	virtual AbstractCommandResponse* executeAsNode(BlockchainNodeHandshake* nodeHandShake, CodablecashNodeInstance* inst, bool suspend) const;

	void addTrxId(const DownloadTransactionEntry* trxEntry);
	void setTransactionType(uint8_t trxType) noexcept;
	void setHeight(uint64_t height) noexcept {
		this->height = height;
	}
	void setBlockHeaderId(const BlockHeaderId* headerId);

	bool isEmpty() const noexcept {
		return this->list->isEmpty();
	}

private:
	const AbstractBlockchainTransaction* getTransaction(const DownloadTransactionEntry *trxEntry, const BlockBody* body) const;

private:
	uint64_t height;
	BlockHeaderId *headerId;
	uint8_t trxType;
	ArrayList<DownloadTransactionEntry>* list;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_DOWNLOADTRANSACTIONSNODECOMMAND_H_ */
