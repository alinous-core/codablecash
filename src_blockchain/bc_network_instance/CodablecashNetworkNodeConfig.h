/*
 * CodablecashNetworkConfig.h
 *
 *  Created on: 2023/08/21
 *      Author: iizuka
 */

#ifndef BC_NETWORK_INSTANCE_CODABLECASHNETWORKNODECONFIG_H_
#define BC_NETWORK_INSTANCE_CODABLECASHNETWORKNODECONFIG_H_

namespace codablecash {

class CodablecashConfig;
class MiningConfig;
class GenesisBalanceConfig;
class NodeIdentifierSource;
class FinalizerConfig;

class CodablecashNetworkNodeConfig {
public:
	CodablecashNetworkNodeConfig(const CodablecashNetworkNodeConfig& inst);
	CodablecashNetworkNodeConfig();
	virtual ~CodablecashNetworkNodeConfig();

	const CodablecashConfig* getSysConfig() const noexcept {
		return this->sysConfig;
	}
	void setSysConfig(const CodablecashConfig* sysConfig) noexcept;

	const MiningConfig* getMinerConfig() const noexcept {
		return this->minerConfig;
	}
	void setMinerConfig(const MiningConfig* minerConfig) noexcept;

	const GenesisBalanceConfig* getGenesisConfig() const noexcept {
		return this->genesisConfig;
	}
	void setGenesisConfig(const GenesisBalanceConfig* genesisConfig) noexcept;

	const FinalizerConfig* getFinalizerConfig() const noexcept {
		return this->finalizerConfig;
	}
	void setFinalizerConfig(const FinalizerConfig* fconfig) noexcept;

	void setPort(int port) noexcept {
		this->port = port;
	}
	int getPort() const noexcept {
		return this->port;
	}
	const NodeIdentifierSource* getNetworkKey() const noexcept {
		return this->networkKeys;
	}
	void setNetworkKey(const NodeIdentifierSource* keys) noexcept;

private:
	CodablecashConfig* sysConfig;
	MiningConfig* minerConfig;
	FinalizerConfig* finalizerConfig;

	GenesisBalanceConfig* genesisConfig;

	int port;
	NodeIdentifierSource* networkKeys;

};

} /* namespace codablecash */

#endif /* BC_NETWORK_INSTANCE_CODABLECASHNETWORKNODECONFIG_H_ */
