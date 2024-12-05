/*
 * P2pDnsManager.cpp
 *
 *  Created on: 2023/09/06
 *      Author: iizuka
 */

#include "bc_p2p_info/P2pDnsManager.h"
#include "bc_p2p_info/P2pNodeDatabase.h"
#include "bc_p2p_info/P2pNodeRecord.h"

#include "bc_p2p_client/P2pClient.h"

#include "base/Exception.h"
#include "base/StackRelease.h"

#include "bc_p2p_cmd_client/DownloadDnsInfoCommand.h"

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "base_thread/SysMutex.h"
#include "base_thread/StackUnlocker.h"

#include "bc/ISystemLogger.h"

#include "bc_p2p_cmd_client/DownloadDnsInfoCommandResponse.h"
namespace codablecash {

P2pDnsManager::P2pDnsManager(const File* baseDir) {
	this->database = new P2pNodeDatabase(baseDir);
	this->mutex = new SysMutex();
}

P2pDnsManager::~P2pDnsManager() {
	delete this->database;
	delete this->mutex;
}

void P2pDnsManager::createBlankDatabase() {
	this->database->createBlankDatabase();
}

void P2pDnsManager::open() {
	StackUnlocker __lock(this->mutex, __FILE__, __LINE__);

	this->database->open();
}

void P2pDnsManager::close() {
	StackUnlocker __lock(this->mutex, __FILE__, __LINE__);

	this->database->close();
}

void P2pDnsManager::importSeeds(int numZone, const P2pNodeRecord *nodeRecord, ISystemLogger *logger) {
	StackUnlocker __lock(this->mutex, __FILE__, __LINE__);

	const UnicodeString* host = nodeRecord->getHost();
	int port = nodeRecord->getPort();

	try{
		for(int i = 0; i != numZone; ++i){
			doImportSeeds(i, host, port, logger);
		}

		// add seed
		this->database->importUnconnectedNode(nodeRecord);
	}
	catch(Exception* e){
		__STP(e);
		logger->logException(e);
	}
}

void P2pDnsManager::doImportSeeds(uint16_t zone, const UnicodeString *host, int port, ISystemLogger *logger) {
	P2pClient client(zone, logger);
	client.connectIpV6(host, port);

	DownloadDnsInfoCommand command(zone);

	client.sign(&command);
	AbstractCommandResponse* response = client.sendCommnad(&command); __STP(response);

	// client close
	client.close();

	DownloadDnsInfoCommandResponse* dnsInfoResponse = dynamic_cast<DownloadDnsInfoCommandResponse*>(response);
	if(dnsInfoResponse != nullptr){
		const ArrayList<P2pNodeRecord>* list = dnsInfoResponse->getRecords();

		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			P2pNodeRecord* rec = list->get(i);

			this->database->importUnconnectedNode(rec);
		}
	}
}

ArrayList<P2pNodeRecord>* P2pDnsManager::getZoneRecords(uint16_t zone, int maxNum) {
	StackUnlocker __lock(this->mutex, __FILE__, __LINE__);

	return this->database->getZoneRecords(zone, maxNum);
}

} /* namespace codablecash */
