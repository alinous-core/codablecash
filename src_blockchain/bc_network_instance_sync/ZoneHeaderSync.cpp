/*
 * ZoneHeaderSync.cpp
 *
 *  Created on: 2023/11/01
 *      Author: iizuka
 */

#include "bc_network_instance_sync/ZoneHeaderSync.h"
#include "bc_network_instance_sync/NetworkNodeSync.h"

#include "bc_network_instance_sync_repo/SyncHeaderRepo.h"
#include "bc_network_instance_sync_repo/HeightHeaderSyncDataList.h"

#include "bc_network_instance/CodablecashNetworkNode.h"

#include "bc/CodablecashNodeInstance.h"

#include "base/StackRelease.h"

#include "base_thread/StackUnlocker.h"
#include "base_thread/SysMutex.h"

#include "bc/ISystemLogger.h"

#include "bc_block/BlockHeader.h"

#include "data_history_data/BlockHeaderTransferData.h"

#include "bc_status_cache/BlockchainController.h"
namespace codablecash {

ZoneHeaderSync::ZoneHeaderSync(uint16_t zone, NetworkNodeSync* parent, ISystemLogger* logger, const UnicodeString* name)
		: AbstractZoneSync(zone, parent, logger, name){

}

ZoneHeaderSync::~ZoneHeaderSync() {

}

void ZoneHeaderSync::importResult() {
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

void ZoneHeaderSync::handleHeader(const BlockHeaderTransferData *data, CodablecashNodeInstance *inst) {
	ISystemLogger* logger = inst->getLogger();

	{
		// AbstractZoneSync::importHeaders()
		UnicodeString __str(L"ZoneBlockchainSync::handleHeader() starts.");
		logger->debugLog(ISystemLogger::DEBUG_TMP_INFO, &__str, __FILE__, __LINE__);
	}

	const BlockHeader* header = data->getHeader();

	BlockchainController* ctrl = inst->getController();
	ctrl->addBlockHeader(header);
}

} /* namespace codablecash */
