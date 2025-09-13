/*
 * MultizoneTestnet.h
 *
 *  Created on: Nov 19, 2024
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_UTILS_TESTNET_MULTIZONETESTNET_H_
#define BLOCKCHAIN_UTILS_TESTNET_MULTIZONETESTNET_H_

#include "base/ArrayList.h"
#include <cstdint>

namespace alinous {
class File;
}
using namespace alinous;

namespace codablecash {

class StackTestPortGetter;
class CodablecashSystemParam;
class ZoneTestnet;
class CodablecashNetworkNodeConfig;
class ISystemLogger;
class TestnetInstanceWrapper;
class IDebugSeeder;

class MultizoneTestnet {
public:
	MultizoneTestnet(StackTestPortGetter* portSel, const File* projectFolder, ISystemLogger* logger, const CodablecashSystemParam* param, const CodablecashNetworkNodeConfig* baseNwconfig);
	virtual ~MultizoneTestnet();


	ZoneTestnet* getZone(uint16_t zone);

	TestnetInstanceWrapper* createInstance(uint16_t zone, const wchar_t* name);


	void startGenesis(uint16_t zone, IDebugSeeder* seeder);
	void startInstancesFrom(uint16_t zone, int fromIndex, IDebugSeeder* seeder);

	void waitForBlockHeight(uint16_t zone, int index, uint64_t height);
	void waitForHeaderHeight(uint16_t zoneSelf, uint16_t zone, int index, uint64_t height);


	void suspendMining(uint16_t zone);
	void resumeMining(uint16_t zone);

private:
	StackTestPortGetter* portSel;
	File* projectFolder;
	ISystemLogger* logger;
	CodablecashSystemParam* param;
	CodablecashNetworkNodeConfig* baseNwconfig;

	ArrayList<ZoneTestnet> zones;
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_UTILS_TESTNET_MULTIZONETESTNET_H_ */
