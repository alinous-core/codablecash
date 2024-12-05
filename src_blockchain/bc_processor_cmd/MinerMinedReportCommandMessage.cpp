/*
 * MinedReportCommandMessage.cpp
 *
 *  Created on: 2023/04/04
 *      Author: iizuka
 */

#include "bc_block/Block.h"

#include "bc_processor/CentralProcessor.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc_p2p_processor/P2pRequestProcessor.h"

#include "bc_processor_cmd/MinerMinedReportCommandMessage.h"

#include "data_history_data/BlockHeaderTransferData.h"

#include "base/StackRelease.h"

#include "bc_network/NodeIdentifierSource.h"
#include "bc_network/NodeIdentifier.h"

#include "bc_p2p/BlochchainP2pManager.h"

#include "bc_p2p_cmd_node_consensus/ReportMinedBlockNodeCommand.h"
#include "bc_p2p_cmd_node_consensus/ReportNonceCalculatedNodeCommand.h"


namespace codablecash {

MinerMinedReportCommandMessage::MinerMinedReportCommandMessage(const Block* block) {
	this->block = new Block(*block);
}

MinerMinedReportCommandMessage::~MinerMinedReportCommandMessage() {
	delete this->block;
}

/**
 * onBlockMined the miner process of this node mined a block
 */
void MinerMinedReportCommandMessage::process(CentralProcessor *processor) {
	BlockchainController* ctrl = processor->getCtrl();
	ctrl->addBlock(this->block);

	// [Network] bload cast
	P2pRequestProcessor* p2pRequestProcessor = processor->getP2pRequestProcessor();
	BlochchainP2pManager* p2pManager = processor->getBlochchainP2pManager();

	// register header for PoS limit
	BlockHeaderTransferData* data = BlockHeaderTransferData::createFromBlock(this->block); __STP(data);
	{
		const BlockHeader* header = data->getHeader();
		uint16_t zone = header->getZone();
		CodablecashSystemParam* config = processor->getCodablecashSystemParam();

		ctrl->registerBlockHeader4Limit(zone, header, config);
	}


	if(p2pRequestProcessor != nullptr && p2pManager != nullptr){
		// notify PoS ticket time limits
		{
			ReportNonceCalculatedNodeCommand cmd;
			const BlockHeader* header = data->getHeader();
			cmd.setHeader(header);

			// sign
			NodeIdentifierSource* nwkey = p2pRequestProcessor->getNetworkKey();
			cmd.sign(nwkey);

			ArrayList<NodeIdentifier> list;
			p2pManager->bloadCastHighPriorityAllZones(&list, &cmd, p2pRequestProcessor);
		}
		// notify headers
		{
			ReportMinedBlockNodeCommand command;
			command.setData(data);

			// sign
			NodeIdentifierSource* nwkey = p2pRequestProcessor->getNetworkKey();
			command.sign(nwkey);

			p2pManager->bloadCastAllZones(nullptr, &command, p2pRequestProcessor);
		}

		// TODO; client notify
	}
}

} /* namespace codablecash */
