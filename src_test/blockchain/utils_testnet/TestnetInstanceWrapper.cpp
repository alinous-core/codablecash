/*
 * TestnetInstanceWrapper.cpp
 *
 *  Created on: Nov 20, 2024
 *      Author: iizuka
 */

#include "TestnetInstanceWrapper.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "base_io/File.h"

#include "bc_network_instance/CodablecashNetworkNodeConfig.h"
#include "bc_network_instance/CodablecashNetworkNode.h"

#include "bc_network/NodeIdentifierSource.h"

#include "bc/CodablecashNodeInstance.h"

#include "bc_status_cache/BlockchainController.h"

#include "IDebugSeeder.h"

namespace codablecash {

TestnetInstanceWrapper::TestnetInstanceWrapper(uint16_t zone, int index) {
	this->zone = zone;
	this->index = index;
	this->baseDir = nullptr;
	this->nwconfig = nullptr;
	this->logger = nullptr;
	this->node = nullptr;
}

TestnetInstanceWrapper::~TestnetInstanceWrapper() {
	if(this->node != nullptr){
		this->node->shutdown();
	}
	delete this->node;

	delete baseDir;
	delete nwconfig;
	this->logger = nullptr;
}

void TestnetInstanceWrapper::init(const wchar_t* name, int port, const File *zoneBaseDir, const CodablecashNetworkNodeConfig *baseNwconfig, ISystemLogger *logger) {
	this->logger = logger;

	UnicodeString seg(L"");
	seg.append((int)this->index);

	this->baseDir = zoneBaseDir->get(&seg);

	this->nwconfig = new CodablecashNetworkNodeConfig(*baseNwconfig);
	this->nwconfig->setCanonicalName(name);
	this->nwconfig->setPort(port);

	NodeIdentifierSource* networkKey =  NodeIdentifierSource::create(); __STP(networkKey);
	this->nwconfig->setNetworkKey(networkKey);

	this->node = new CodablecashNetworkNode(this->baseDir, this->nwconfig, this->logger);
	this->node->setNodeName(name);
}

void TestnetInstanceWrapper::setGenesisBlockConfig(const GenesisBalanceConfig* gconfig) {
	this->nwconfig->setGenesisConfig(gconfig);
}

void TestnetInstanceWrapper::setMinerConfig(const MiningConfig *minerConfig) {
	this->nwconfig->setMinerConfig(minerConfig);
}

void TestnetInstanceWrapper::setFinalizerConfig(const FinalizerConfig *fconfig) {
	this->nwconfig->setFinalizerConfig(fconfig);
}

void TestnetInstanceWrapper::initGenesis() {
	this->node->setNetworkConfig(this->nwconfig);
	this->node->generateNetwork(this->zone);
}

void TestnetInstanceWrapper::initBlank() {
	this->node->setNetworkConfig(this->nwconfig);
	this->node->initBlank(this->zone);
}

void TestnetInstanceWrapper::start(IDebugSeeder* seeder) {
	this->node->startNetwork(seeder, false);

	this->node->startMiner();
	this->node->startStakePool();
}

uint64_t TestnetInstanceWrapper::getHeight() {
	CodablecashNodeInstance* inst = this->node->getInstance();

	BlockchainController* ctrl = inst->getController();
	uint64_t h = ctrl->getHeadHeight(this->zone);

	return h;
}

} /* namespace codablecash */
