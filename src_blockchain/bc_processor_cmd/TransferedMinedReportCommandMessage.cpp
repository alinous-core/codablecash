/*
 * TransferedMinedReportCommandMessage.cpp
 *
 *  Created on: 2023/11/23
 *      Author: iizuka
 */

#include "bc_processor_cmd/TransferedMinedReportCommandMessage.h"

#include "data_history_data/BlockHeaderTransferData.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_processor/CentralProcessor.h"

#include "bc_p2p_processor/P2pRequestProcessor.h"

#include "bc_p2p/BlochchainP2pManager.h"
#include "bc_p2p/BlockchainNodeHandshake.h"
#include "bc_p2p/BlockchainNodeHandshakeException.h"
#include "bc_p2p/StackHandshakeReleaser.h"

#include "bc_blockstore/CodablecashBlockchain.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc_block/BlockHeader.h"
#include "bc_block/Block.h"
#include "bc_block_body/BlockBody.h"

#include "bc_p2p_cmd_node/DownloadOmittedBlockBodyNodeCommand.h"
#include "bc_p2p_cmd_node/DownloadOmittedBlockBodyNodeCommandResponse.h"
#include "bc_p2p_cmd_node/ReportMinedBlockNodeCommand.h"

#include "bc/ExceptionThrower.h"
#include "bc/ISystemLogger.h"

#include "base/StackRelease.h"

#include "bc_block_validator/BlockValidator.h"

#include "bc_memorypool/MemoryPool.h"

#include "bc_trx/AbstractBlockchainTransaction.h"

#include "bc_p2p_cmd_node/DownloadTransactionsNodeCommand.h"

#include "bc_network/NodeIdentifierSource.h"

#include "bc_p2p_cmd_node/DownloadTransactionsNodeCommandResponse.h"

#include "pubsub/P2pHandshake.h"

#include "base_thread/StackUnlocker.h"

#include "base_thread/SysMutex.h"
namespace codablecash {

TransferedMinedReportCommandMessage::TransferedMinedReportCommandMessage() {
	this->data = nullptr;
	this->nodeId = nullptr;
}

TransferedMinedReportCommandMessage::~TransferedMinedReportCommandMessage() {
	delete this->data;
	delete this->nodeId;
}

void TransferedMinedReportCommandMessage::setData(const BlockHeaderTransferData *data) {
	delete this->data;
	this->data = dynamic_cast<BlockHeaderTransferData*>(data->copyData());
}

void TransferedMinedReportCommandMessage::setNodeId(const NodeIdentifier *nodeId) {
	delete this->nodeId;
	this->nodeId = dynamic_cast<NodeIdentifier*>(nodeId->copyData());
}

void TransferedMinedReportCommandMessage::process(CentralProcessor *processor) {
	P2pRequestProcessor* requestProcessor = processor->getP2pRequestProcessor();
	BlochchainP2pManager* p2pManager = processor->getBlochchainP2pManager();
	ISystemLogger* logger = processor->getLogger();
	MemoryPool* memPool = processor->getMemoryPool();
	CodablecashConfig* config = processor->getConfig();

	BlockchainController* ctrl = processor->getCtrl();
	uint16_t zoneSelf = ctrl->getZoneSelf();

	const BlockHeader* header = this->data->getHeader();
	uint16_t zone = header->getZone();

	bool dataAdded = false;
	if(zoneSelf == zone){
		NodeIdentifierSource* networkKey = requestProcessor->getNetworkKey();
		dataAdded = importBlock(memPool, ctrl, p2pManager, networkKey, logger, config);
	}else{
		dataAdded = importHeader(ctrl);
	}

	if(dataAdded){
		NodeIdentifierSource* networkKey = requestProcessor->getNetworkKey();
		ReportMinedBlockNodeCommand command;
		command.setData(this->data);

		command.sign(networkKey);

		p2pManager->bloadCastAllZones(this->nodeId, &command, requestProcessor);

		// TODO; client notify
	}
}

bool TransferedMinedReportCommandMessage::importHeader(BlockchainController *ctrl) {
	const BlockHeader* header = this->data->getHeader();

	return ctrl->addBlockHeader(header);
}

bool TransferedMinedReportCommandMessage::importBlock(MemoryPool* memPool, BlockchainController *ctrl, BlochchainP2pManager *p2pManager
		, NodeIdentifierSource* networkKey ,ISystemLogger* logger, CodablecashConfig* config) {
	const BlockHeader* header = this->data->getHeader();
	uint64_t height = header->getHeight();
	const BlockHeaderId* headerId = header->getId();


	DownloadOmittedBlockBodyNodeCommand command;
	command.setZone(header->getZone());
	command.setHeight(height);
	command.setHeaderId(headerId);

	// sign
	command.sign(networkKey);

	// download entire block command
	DownloadOmittedBlockBodyNodeCommandResponse* omittedBodyResponse = nullptr;
	{
		BlockchainNodeHandshake* handshake = p2pManager->getNodeHandshakeByNodeId(this->nodeId);
		ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(handshake == nullptr, L"Node connection has alrealy closed.", __FILE__, __LINE__);

		StackHandshakeReleaser __releaser(handshake);

		AbstractCommandResponse* response = handshake->sendCommnad(&command); __STP(response);
		omittedBodyResponse = dynamic_cast<DownloadOmittedBlockBodyNodeCommandResponse*>(response);

		ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(omittedBodyResponse == nullptr, L"Respose error.", __FILE__, __LINE__);
		__STP_MV(response);

		UnicodeString* message = omittedBodyResponse->toString(); __STP(message);
		logger->debugLog(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE, message, __FILE__, __LINE__);
	}
	__STP(omittedBodyResponse);

	// download entire block command
	const OmittedBlockBody* obody = omittedBodyResponse->getBody();

	BlockBody* body = obody->toBlockBody(height, headerId, this);	__STP(body);

	Block block(dynamic_cast<BlockHeader*>(header->copyData()), __STP_MV(body));

	BlockValidator validator(&block, config, memPool, ctrl);
	validator.validate();

	return ctrl->addBlock(&block);
}

MemPoolTransaction* TransferedMinedReportCommandMessage::begin() {
	MemoryPool* memPool = this->processor->getMemoryPool();

	return memPool->begin();
}

DownloadTransactionsNodeCommandResponse* TransferedMinedReportCommandMessage::downloadTransactions(const DownloadTransactionsNodeCommand *command) const {
	if(command->isEmpty()){
		return new DownloadTransactionsNodeCommandResponse();
	}

	P2pRequestProcessor* requestProcessor = processor->getP2pRequestProcessor();
	BlochchainP2pManager* p2pManager = processor->getBlochchainP2pManager();
	ISystemLogger* logger = processor->getLogger();

	BlockchainNodeHandshake* handshake = p2pManager->getNodeHandshakeByNodeId(this->nodeId);
	ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(handshake == nullptr, L"Node connection has alrealy closed.", __FILE__, __LINE__);

	StackHandshakeReleaser __releaser(handshake);

	P2pHandshake* p2pHandshake = handshake->getHandshake();

	SysMutex* mutex = handshake->getSysMutex();
	StackUnlocker __lock(mutex);

	return __downloadTransactions(command, p2pHandshake, logger);
}

DownloadTransactionsNodeCommandResponse* TransferedMinedReportCommandMessage::__downloadTransactions(
		const DownloadTransactionsNodeCommand *command, P2pHandshake* handshake, ISystemLogger *logger) {
	DownloadTransactionsNodeCommandResponse* ret = nullptr;
	{
		ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(handshake == nullptr, L"Node connection has alrealy closed.", __FILE__, __LINE__);

		AbstractCommandResponse* response = handshake->publishCommand(command); __STP(response);
		ret = dynamic_cast<DownloadTransactionsNodeCommandResponse*>(response);

		ExceptionThrower<BlockchainNodeHandshakeException>::throwExceptionIfCondition(ret == nullptr, L"Respose error.", __FILE__, __LINE__);


		UnicodeString* message = ret->toString(); __STP(message);
		logger->debugLog(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE, message, __FILE__, __LINE__);

		__STP_MV(response);
	}

	return ret;
}

NodeIdentifierSource* TransferedMinedReportCommandMessage::getNetworkKey() const noexcept {
	P2pRequestProcessor* requestProcessor = processor->getP2pRequestProcessor();
	NodeIdentifierSource* networkKey = requestProcessor->getNetworkKey();
	return networkKey;
}

} /* namespace codablecash */
