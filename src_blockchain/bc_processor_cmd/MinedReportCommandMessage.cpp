/*
 * MinedReportCommandMessage.cpp
 *
 *  Created on: 2023/04/04
 *      Author: iizuka
 */

#include "bc_processor_cmd/MinedReportCommandMessage.h"

#include "bc_p2p_cmd_node/ReportMinedBlockNodeCommand.h"

#include "bc_block/Block.h"

#include "bc_processor/CentralProcessor.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc_p2p_processor/P2pRequestProcessor.h"

#include "data_history_data/BlockHeaderTransferData.h"

#include "base/StackRelease.h"

#include "bc_network/NodeIdentifierSource.h"

#include "bc_p2p/BlochchainP2pManager.h"


namespace codablecash {

MinedReportCommandMessage::MinedReportCommandMessage(const Block* block) {
	this->block = new Block(*block);
}

MinedReportCommandMessage::~MinedReportCommandMessage() {
	delete this->block;
}

/**
 * onBlockMined the miner process of this node mined a block
 */
void MinedReportCommandMessage::process(CentralProcessor *processor) {
	BlockchainController* ctrl = processor->getCtrl();
	ctrl->addBlock(this->block);

	// [Network] bload cast
	P2pRequestProcessor* p2pRequestProcessor = processor->getP2pRequestProcessor();
	BlochchainP2pManager* p2pManager = processor->getBlochchainP2pManager();

	if(p2pRequestProcessor != nullptr && p2pManager != nullptr){
		ReportMinedBlockNodeCommand command;

		BlockHeaderTransferData* data = BlockHeaderTransferData::createFromBlock(this->block); __STP(data);
		command.setData(data);

		// sign
		NodeIdentifierSource* nwkey = p2pRequestProcessor->getNetworkKey();
		command.sign(nwkey);

		p2pManager->bloadCastAllZones(nullptr, &command, p2pRequestProcessor);

		// TODO; client notify
	}
}

} /* namespace codablecash */
