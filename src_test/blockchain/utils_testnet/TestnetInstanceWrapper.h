/*
 * TestnetInstanceWrapper.h
 *
 *  Created on: Nov 20, 2024
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_UTILS_TESTNET_TESTNETINSTANCEWRAPPER_H_
#define BLOCKCHAIN_UTILS_TESTNET_TESTNETINSTANCEWRAPPER_H_
#include <cstdint>

namespace alinous {
class File;
}
using namespace alinous;

namespace codablecash {

class CodablecashNetworkNodeConfig;
class ISystemLogger;
class CodablecashNetworkNode;
class GenesisBalanceConfig;
class MiningConfig;
class FinalizerConfig;
class IDebugSeeder;

class TestnetInstanceWrapper {
public:
	TestnetInstanceWrapper(uint16_t zone, int index);
	virtual ~TestnetInstanceWrapper();

	void init(const wchar_t* name, int port, const File* zoneBaseDir, const CodablecashNetworkNodeConfig* baseNwconfig, ISystemLogger* logger);

	void setGenesisBlockConfig(const GenesisBalanceConfig* gconfig);

	void setMinerConfig(const MiningConfig* minerConfig);
	void setFinalizerConfig(const FinalizerConfig* config);

	void initGenesis();
	void initBlank();

	void start(IDebugSeeder* seeder, bool pending);
	void syncNetwork();

	uint64_t getHeight();

private:
	uint16_t zone;
	int index;
	File* baseDir;
	CodablecashNetworkNodeConfig* nwconfig;
	ISystemLogger* logger;

	CodablecashNetworkNode* node;

};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_UTILS_TESTNET_TESTNETINSTANCEWRAPPER_H_ */
