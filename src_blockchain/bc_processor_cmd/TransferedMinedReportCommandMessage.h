/*
 * TransferedMinedReportCommandMessage.h
 *
 *  Created on: 2023/11/23
 *      Author: iizuka
 */

#ifndef BC_PROCESSOR_CMD_TRANSFEREDMINEDREPORTCOMMANDMESSAGE_H_
#define BC_PROCESSOR_CMD_TRANSFEREDMINEDREPORTCOMMANDMESSAGE_H_

#include "bc_processor/AbstractCentralProcessorCommandMessage.h"

#include "bc_block_body/OmittedBlockBody.h"

namespace codablecash {

class BlockHeaderTransferData;
class NodeIdentifier;
class BlockchainController;
class BlochchainP2pManager;
class NodeIdentifierSource;
class ISystemLogger;
class CodablecashSystemParam;
class MemoryPool;
class P2pHandshake;


class TransferedMinedReportCommandMessage : public AbstractCentralProcessorCommandMessage, public IBlockBodyFetcher {
public:
	TransferedMinedReportCommandMessage();
	virtual ~TransferedMinedReportCommandMessage();

	void setData(const BlockHeaderTransferData* data);
	void setNodeId(const NodeIdentifier* nodeId);

	virtual MemPoolTransaction* begin();
	virtual DownloadTransactionsNodeCommandResponse* downloadTransactions(const DownloadTransactionsNodeCommand* command) const;
	virtual NodeIdentifierSource* getNetworkKey() const noexcept;

	static DownloadTransactionsNodeCommandResponse* __downloadTransactions(const DownloadTransactionsNodeCommand* command,
			P2pHandshake* handshake, ISystemLogger* logger);

protected:
	virtual void process(CentralProcessor* processor);

private:
	bool importHeader(BlockchainController* ctrl);
	bool importBlock(MemoryPool* memPool, BlockchainController* ctrl, BlochchainP2pManager* p2pManager, NodeIdentifierSource* networkKey
			,ISystemLogger* logger, CodablecashSystemParam* config);

private:
	NodeIdentifier* nodeId;
	BlockHeaderTransferData* data;
};

} /* namespace codablecash */

#endif /* BC_PROCESSOR_CMD_TRANSFEREDMINEDREPORTCOMMANDMESSAGE_H_ */
