/*
 * CodablecashNetworkConfig.cpp
 *
 *  Created on: 2023/08/21
 *      Author: iizuka
 */

#include "bc_block_generator/MiningConfig.h"

#include "bc/CodablecashConfig.h"

#include "bc_network_instance/CodablecashNetworkNodeConfig.h"
#include "bc_network_instance/GenesisBalanceConfig.h"
#include "bc_network_instance/FinalizerConfig.h"

#include "bc_network/NodeIdentifierSource.h"


namespace codablecash {

CodablecashNetworkNodeConfig::CodablecashNetworkNodeConfig(const CodablecashNetworkNodeConfig &inst) {
	this->sysConfig = new CodablecashConfig(*inst.sysConfig);
	this->minerConfig = inst.minerConfig != nullptr ? new MiningConfig(*inst.minerConfig) : nullptr;
	this->finalizerConfig = inst.finalizerConfig != nullptr ? new FinalizerConfig(*inst.finalizerConfig) : nullptr;

	this->genesisConfig = inst.genesisConfig != nullptr ? new GenesisBalanceConfig(*inst.genesisConfig) : nullptr;

	this->port = inst.port;
	this->networkKeys = inst.networkKeys != nullptr ? dynamic_cast<NodeIdentifierSource*>(inst.networkKeys->copyData()) : nullptr;
}

CodablecashNetworkNodeConfig::CodablecashNetworkNodeConfig() {
	this->sysConfig = new CodablecashConfig();
	this->minerConfig = nullptr;
	this->finalizerConfig = nullptr;
	this->genesisConfig = nullptr;

	this->port = 0;
	this->networkKeys = NodeIdentifierSource::create();
}

CodablecashNetworkNodeConfig::~CodablecashNetworkNodeConfig() {
	delete this->sysConfig;
	delete this->minerConfig;
	delete this->finalizerConfig;
	delete this->genesisConfig;
	delete this->networkKeys;
}

void CodablecashNetworkNodeConfig::setSysConfig(const CodablecashConfig *sysConfig) noexcept {
	delete this->sysConfig;
	this->sysConfig = new CodablecashConfig(*sysConfig);
}

void CodablecashNetworkNodeConfig::setMinerConfig(const MiningConfig *minerConfig) noexcept {
	delete this->minerConfig;
	this->minerConfig = new MiningConfig(*minerConfig);
}

void CodablecashNetworkNodeConfig::setGenesisConfig(	const GenesisBalanceConfig *genesisConfig) noexcept {
	delete this->genesisConfig;
	this->genesisConfig = new GenesisBalanceConfig(*genesisConfig);
}

void CodablecashNetworkNodeConfig::setFinalizerConfig(const FinalizerConfig *fconfig) noexcept {
	delete this->finalizerConfig;
	this->finalizerConfig = new FinalizerConfig(*fconfig);
}

void CodablecashNetworkNodeConfig::setNetworkKey(const NodeIdentifierSource *keys) noexcept {
	delete this->networkKeys;
	this->networkKeys = dynamic_cast<NodeIdentifierSource*>(keys->copyData());
}

} /* namespace codablecash */
