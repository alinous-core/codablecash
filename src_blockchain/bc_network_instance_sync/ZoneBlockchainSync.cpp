/*
 * ZoneBlockchainSync.cpp
 *
 *  Created on: 2023/11/01
 *      Author: iizuka
 */

#include "bc_network_instance_sync/ZoneBlockchainSync.h"
#include "bc_network_instance_sync/NetworkNodeSync.h"
#include "bc_network_instance_sync/ZoneBlockValidator.h"

#include "bc_network_instance_sync_repo/SyncHeaderRepo.h"
#include "bc_network_instance_sync_repo/HeightHeaderSyncDataList.h"

#include "bc_network_instance/CodablecashNetworkNode.h"

#include "bc/CodablecashNodeInstance.h"
#include "bc/ExceptionThrower.h"

#include "base/StackRelease.h"

#include "base_thread/StackUnlocker.h"
#include "base_thread/SysMutex.h"

#include "data_history_data/BlockHeaderTransferData.h"

#include "bc_p2p_cmd_node/DownloadBlockNodeCommand.h"
#include "bc_p2p_cmd_node/DownloadBlockNodeCommandResponse.h"

#include "bc_p2p_cmd_node_access/RetryableNode2NodeAccess.h"

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "bc_block/Block.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc/ISystemLogger.h"


namespace codablecash {

ZoneBlockchainSync::ZoneBlockchainSync(uint16_t zone, NetworkNodeSync* parent, ISystemLogger* logger, const UnicodeString* name)
		: AbstractZoneSync(zone, parent, logger, name){

}

ZoneBlockchainSync::~ZoneBlockchainSync() {

}

void ZoneBlockchainSync::importResult() {
	StackUnlocker __lock(this->headerDataMutex, __FILE__, __LINE__);

	CodablecashNetworkNode* node = this->parent->getCodablecashNetworkNode();
	CodablecashNodeInstance* inst = node->getInstance();

	while(!this->headerDataStore->isEmpty()){
		HeightHeaderSyncDataList* datalist = this->headerDataStore->fetchFirst(); __STP(datalist);

		const ArrayList<BlockHeaderTransferData>* list = datalist->getList();
		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			const BlockHeaderTransferData* data = list->get(i);

			handleHeader(data, inst);
		}
	}
}

void ZoneBlockchainSync::handleHeader(const BlockHeaderTransferData *data, CodablecashNodeInstance* inst) {
	ISystemLogger* logger = inst->getLogger();

	{
		// AbstractZoneSync::importHeaders()
		UnicodeString __str(L"ZoneBlockchainSync::handleHeader() starts.");
		this->logger->debugLog(ISystemLogger::DEBUG_TMP_INFO, &__str, __FILE__, __LINE__);
	}

	const BlockHeader* header = data->getHeader();
	uint16_t zone = header->getZone();
	uint64_t height = header->getHeight();
	const BlockHeaderId* id = header->getId();

	DownloadBlockNodeCommand command;
	command.setZone(zone);
	command.setHeight(height);
	command.setHeaderId(id);

	NodeIdentifierSource* source = inst->getNetworkKey();
	command.sign(source);

	{
		BlochchainP2pManager* p2pManager = inst->getBlochchainP2pManager();

		ZoneBlockValidator validator(zone, height, id);
		RetryableNode2NodeAccess access(zone, &command, 10, p2pManager, &validator, logger);

		AbstractCommandResponse* response = access.access(); __STP(response);
		if(response != nullptr){
			UnicodeString* message = response->toString(); __STP(message);
			this->logger->debugLog(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE, message, __FILE__, __LINE__);
		}
		DownloadBlockNodeCommandResponse* dlresponse = dynamic_cast<DownloadBlockNodeCommandResponse*>(response);

		Block* block = dlresponse->getBlock();
		addBlock(block, inst);
	}

	{
		// AbstractZoneSync::importHeaders()
		UnicodeString __str(L"ZoneBlockchainSync::handleHeader() ends.");
		this->logger->debugLog(ISystemLogger::DEBUG_TMP_INFO, &__str, __FILE__, __LINE__);
	}
}


void ZoneBlockchainSync::addBlock(const Block *block, CodablecashNodeInstance *inst) {
	BlockchainController* ctrl = inst->getController();

	ctrl->addBlock(block);
}

} /* namespace codablecash */
