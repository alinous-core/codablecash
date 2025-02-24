/*
 * CodablecashNetworkConfig.cpp
 *
 *  Created on: 2023/08/21
 *      Author: iizuka
 */

#include "bc/CodablecashSystemParam.h"
#include "bc_block_generator/MiningConfig.h"

#include "bc_network_instance/CodablecashNetworkNodeConfig.h"
#include "bc_network_instance/GenesisBalanceConfig.h"
#include "bc_network_instance/FinalizerConfig.h"

#include "bc_network/NodeIdentifierSource.h"
#include "bc_network/NodeIdentifier.h"

#include "base/StackRelease.h"


namespace codablecash {

CodablecashNetworkNodeConfig::CodablecashNetworkNodeConfig(const CodablecashNetworkNodeConfig &inst) {
	this->sysConfig = new CodablecashSystemParam(*inst.sysConfig);
	this->minerConfig = inst.minerConfig != nullptr ? new MiningConfig(*inst.minerConfig) : nullptr;
	this->finalizerConfig = inst.finalizerConfig != nullptr ? new FinalizerConfig(*inst.finalizerConfig) : nullptr;

	this->genesisConfig = inst.genesisConfig != nullptr ? new GenesisBalanceConfig(*inst.genesisConfig) : nullptr;

	this->host = inst.host != nullptr ? new UnicodeString(inst.host) : nullptr;
	this->port = inst.port;
	this->networkKeys = inst.networkKeys != nullptr ? dynamic_cast<NodeIdentifierSource*>(inst.networkKeys->copyData()) : nullptr;

	this->canonicalName = inst.canonicalName != nullptr ? new UnicodeString(inst.canonicalName) : nullptr;
}

CodablecashNetworkNodeConfig::CodablecashNetworkNodeConfig() {
	this->sysConfig = new CodablecashSystemParam();
	this->minerConfig = nullptr;
	this->finalizerConfig = nullptr;
	this->genesisConfig = nullptr;

	this->host = nullptr;
	this->port = 0;
	this->networkKeys = NodeIdentifierSource::create();
	this->canonicalName = nullptr;
}

CodablecashNetworkNodeConfig::~CodablecashNetworkNodeConfig() {
	delete this->sysConfig;
	delete this->minerConfig;
	delete this->finalizerConfig;
	delete this->genesisConfig;
	delete this->networkKeys;
	delete this->host;
	delete this->canonicalName;
}

void CodablecashNetworkNodeConfig::setSysConfig(const CodablecashSystemParam *sysConfig) noexcept {
	delete this->sysConfig;
	this->sysConfig = new CodablecashSystemParam(*sysConfig);
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

void CodablecashNetworkNodeConfig::setOriginalNwtworkKey() noexcept {
	NodeIdentifierSource* keys = NodeIdentifierSource::create(); __STP(keys);
	setNetworkKey(keys);
}

NodeIdentifier CodablecashNetworkNodeConfig::getNodeId() const noexcept {
	return this->networkKeys->toNodeIdentifier();
}

void CodablecashNetworkNodeConfig::setCanonicalName(const wchar_t *cname) noexcept {
	delete this->canonicalName;
	this->canonicalName = new UnicodeString(cname);
}

void CodablecashNetworkNodeConfig::setHost(const UnicodeString *host) {
	delete this->host;
	this->host = new UnicodeString(host);
}

} /* namespace codablecash */
