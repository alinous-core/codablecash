/*
 * OmittedBlockBodyFixer.h
 *
 *  Created on: Sep 4, 2025
 *      Author: iizuka
 */

#ifndef BC_BLOCK_BODY_OMITTEDBLOCKBODYFIXER_H_
#define BC_BLOCK_BODY_OMITTEDBLOCKBODYFIXER_H_
#include <cstdint>

#include "base/HashMap.h"

#include "bc_block_body/IOmittedBlockBodyFixer.h"

using alinous::HashMap;

namespace codablecash {

class NodeIdentifier;
class MemPoolTransaction;
class MemoryPool;
class DownloadTransactionsNodeCommand;
class BlochchainP2pManager;
class BlockHeaderId;
class DownloadTransactionEntry;
class NodeIdentifierSource;
class ISystemLogger;
class DownloadTransactionsNodeCommandResponse;
class P2pHandshake;
class AbstractBlockchainTransaction;
class TransactionId;


class OmittedBlockBodyFixer : public IOmittedBlockBodyFixer {
public:
	OmittedBlockBodyFixer(const NodeIdentifier* nodeId, MemoryPool* memPool, BlochchainP2pManager* p2pManager, const BlockHeaderId* headerId, uint64_t height, NodeIdentifierSource* source);
	virtual ~OmittedBlockBodyFixer();

	virtual MemPoolTransaction* begin();
	virtual void addDownloadTransactionEntry(const DownloadTransactionEntry* entry);
	virtual void downloadTransactions(ISystemLogger *logger);
	virtual const AbstractBlockchainTransaction* get(const TransactionId* trxId) const noexcept;

	static DownloadTransactionsNodeCommandResponse* __downloadTransactions(const DownloadTransactionsNodeCommand* command,
			P2pHandshake* handshake, ISystemLogger* logger);

private:
	NodeIdentifier* nodeId; // source of ommitted header
	MemoryPool* memPool;
	BlochchainP2pManager* p2pManager;

	DownloadTransactionsNodeCommand* command;

	NodeIdentifierSource* source;

	HashMap<TransactionId, AbstractBlockchainTransaction>* map;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_BODY_OMITTEDBLOCKBODYFIXER_H_ */
