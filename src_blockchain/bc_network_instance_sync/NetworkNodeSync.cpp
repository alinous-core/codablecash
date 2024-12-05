/*
 * NetworkNodeSync.cpp
 *
 *  Created on: 2023/11/01
 *      Author: iizuka
 */

#include "bc_network_instance_sync/NetworkNodeSync.h"
#include "bc_network_instance_sync/AbstractZoneSync.h"
#include "bc_network_instance_sync/MemPoolSync.h"

#include "bc_network_instance/CodablecashNetworkNode.h"

#include "bc/CodablecashNodeInstance.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "base_io/File.h"


namespace codablecash {

NetworkNodeSync::NetworkNodeSync(CodablecashNetworkNode* node) {
	this->node = node;
	this->zones = new ArrayList<AbstractZoneSync>();
	this->zoneSelf = 0;
	this->tmpSyncDir = nullptr;
}

NetworkNodeSync::~NetworkNodeSync() {
	this->zones->deleteElements();
	delete this->zones;

	this->node = nullptr;

	if(this->tmpSyncDir != nullptr){
		this->tmpSyncDir->deleteDir();
	}
	delete this->tmpSyncDir;
}

void NetworkNodeSync::sync() {
	CodablecashNodeInstance* inst = this->node->getInstance();
	ISystemLogger* logger = inst->getLogger();

	{
		File* tmpDir = inst->getTempCacheDir(); __STP(tmpDir);

		// reset
		delete this->tmpSyncDir;
		this->tmpSyncDir = tmpDir->get(SYNC_DIR);
		this->tmpSyncDir->deleteDir();
		this->tmpSyncDir->mkdirs();
	}

	// sync mempool at first
	UnicodeString memSyncThreadName(L"MemPoolSync");
	MemPoolSync memPoolSync(this, logger, &memSyncThreadName);
	memPoolSync.start();


	// sync blockchain or header
	uint16_t numZones = inst->getNumZones();
	this->zoneSelf = inst->getZoneSelf();

	for(int i = 0; i != numZones; ++i){
		AbstractZoneSync* zoneSync = AbstractZoneSync::createZone(i, zoneSelf, this, logger);
		this->zones->addElement(zoneSync);

		zoneSync->start();
	}

	for(int i = 0; i != numZones; ++i){
		AbstractZoneSync* zoneSync = this->zones->get(i);

		zoneSync->join();
	}
	memPoolSync.join();
}

} /* namespace codablecash */
