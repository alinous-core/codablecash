/*
 * CodablecashNetworkConfig.h
 *
 *  Created on: 2023/08/21
 *      Author: iizuka
 */

#ifndef BC_NETWORK_INSTANCE_CODABLECASHNETWORKNODECONFIG_H_
#define BC_NETWORK_INSTANCE_CODABLECASHNETWORKNODECONFIG_H_

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class CodablecashSystemParam;
class MiningConfig;
class GenesisBalanceConfig;
class NodeIdentifierSource;
class FinalizerConfig;
class NodeIdentifier;

class CodablecashNetworkNodeConfig {
public:
	CodablecashNetworkNodeConfig(const CodablecashNetworkNodeConfig& inst);
	CodablecashNetworkNodeConfig();
	virtual ~CodablecashNetworkNodeConfig();

	const CodablecashSystemParam* getSysConfig() const noexcept {
		return this->sysConfig;
	}
	void setSysConfig(const CodablecashSystemParam* sysConfig) noexcept;

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

	void setHost(const UnicodeString* host);
	const UnicodeString* getHost() const noexcept {
		return this->host;
	}

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
	void setOriginalNwtworkKey() noexcept;

	NodeIdentifier getNodeId() const noexcept;

	void setCanonicalName(const wchar_t* cname) noexcept;
	const UnicodeString* getCanonicalName() const noexcept {
		return this->canonicalName;
	}

private:
	CodablecashSystemParam* sysConfig;
	MiningConfig* minerConfig;
	FinalizerConfig* finalizerConfig;

	GenesisBalanceConfig* genesisConfig;

	int port;
	UnicodeString* host;
	NodeIdentifierSource* networkKeys;

	UnicodeString* canonicalName;

};

} /* namespace codablecash */

#endif /* BC_NETWORK_INSTANCE_CODABLECASHNETWORKNODECONFIG_H_ */
