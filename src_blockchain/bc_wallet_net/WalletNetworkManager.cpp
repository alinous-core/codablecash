/*
 * WalletNetworkManager.cpp
 *
 *  Created on: Feb 17, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net/WalletNetworkManager.h"
#include "bc_wallet_net/WalletConnectionManager.h"
#include "bc_wallet_net/WalletNetworkNodeCandidates.h"

#include "bc_network_instance/INetworkSeeder.h"

#include "bc_p2p_info/P2pNodeRecord.h"

#include "base/ArrayList.h"
#include "base/StackRelease.h"

#include "bc_p2p_client/P2pClient.h"

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "bc/ISystemLogger.h"

#include "base/Exception.h"

#include "bc_p2p_cmd_client/DownloadDnsInfoCommandResponse.h"
#include "bc_p2p_cmd_client/DownloadDnsInfoCommand.h"

#include "osenv/funcs.h"


namespace codablecash {

WalletNetworkManager::WalletNetworkManager(uint16_t defaultZone, ISystemLogger* logger, NetworkClientCommandProcessor* clientCommandProcessor) {
	this->seeder = nullptr;
	this->logger = logger;

	this->nodesCandidates = new WalletNetworkNodeCandidates();
	this->connectionManager = new WalletConnectionManager(defaultZone, clientCommandProcessor);

	this->maxNumConnection = MAX_CONNECTIONS;
}

WalletNetworkManager::~WalletNetworkManager() {
	delete this->nodesCandidates;
	delete this->connectionManager;

	this->seeder = nullptr;
}

void WalletNetworkManager::setSeeder(INetworkSeeder *seeder) noexcept {
	this->seeder = seeder;
}

void WalletNetworkManager::maintainNetwork(const ArrayList<BloomFilter512>* filters) {
	// make candodates list
	{
		ArrayList<P2pNodeRecord>* list = this->seeder->getSeedNodes(); __STP(list);
		list->setDeleteOnExit();

		int numZones = this->seeder->getNumZones();

		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			P2pNodeRecord* seedRec = list->get(i);

			importSeeds(numZones, seedRec);
		}
	}


	ArrayList<const P2pNodeRecord>* list = this->nodesCandidates->getCandidatesList(); __STP(list);

	int maxLoop = list->size();
	int conNum = this->connectionManager->getNumConnection();
	for(int i = 0; i != maxLoop && conNum <= this->maxNumConnection; ++i){
		int curSize = list->size();

		int pos = selectRandom(curSize);
		const P2pNodeRecord* rec = list->remove(pos);

		int protocol = rec->getProtocol();
		const UnicodeString* host = rec->getHost();
		int port = rec->getPort();
		const NodeIdentifier* nodeId = rec->getNodeIdentifier();
		uint16_t zone = rec->getZone();

		if(!this->connectionManager->hasNodeId(nodeId)){
			this->connectionManager->connect(protocol, host, port, nodeId, zone, this->logger, filters);
		}

		conNum = this->connectionManager->getNumConnection();
	}
}

int WalletNetworkManager::selectRandom(int curSize) const noexcept {
	int randomNum = Os::random();

	return randomNum % curSize;
}

void WalletNetworkManager::importSeeds(int numZones, const P2pNodeRecord *seedRec) {
	const UnicodeString* host = seedRec->getHost();
	int port = seedRec->getPort();
	int protocol = seedRec->getProtocol();

	try{
		for(int i = 0; i != numZones; ++i){
			doInitNodeCandidates(i, host, port, protocol);
		}

		// add seed
		if(this->nodesCandidates->size() < 20){
			this->nodesCandidates->addRecord(seedRec);
		}
	}
	catch(Exception* e){
		__STP(e);
		this->logger->logException(e);
	}
}



void WalletNetworkManager::doInitNodeCandidates(uint16_t zone, const UnicodeString *host, int port, int protocol) {
	P2pClient client(zone, this->logger);

	client.connect(protocol, host, port);

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

			this->nodesCandidates->addRecord(rec);
		}
	}
}


} /* namespace codablecash */
