/*
 * ZoneTestnet.h
 *
 *  Created on: Nov 20, 2024
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_UTILS_TESTNET_ZONETESTNET_H_
#define BLOCKCHAIN_UTILS_TESTNET_ZONETESTNET_H_

#include "base/ArrayList.h"
#include <cstdint>

namespace alinous {
class File;
}
using namespace alinous;

namespace codablecash {

class TestnetInstanceWrapper;
class CodablecashNetworkNodeConfig;
class ISystemLogger;
class IDebugSeeder;

class ZoneTestnet {
public:
	ZoneTestnet(uint16_t zone, const File* projectFolder);
	virtual ~ZoneTestnet();

	TestnetInstanceWrapper* createTestnetInstanceWrapper(const wchar_t* name, int port, const CodablecashNetworkNodeConfig* baseNwconfig, ISystemLogger* logger);

	void startGenesis(IDebugSeeder* seeder);
	void startFrom(IDebugSeeder* seeder, int fromIndex);

	TestnetInstanceWrapper* getInstance(int index) const noexcept;

private:
	uint16_t zone;
	File* zoneFolder;

	ArrayList<TestnetInstanceWrapper> instances;
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_UTILS_TESTNET_ZONETESTNET_H_ */
