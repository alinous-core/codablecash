/*
 * TestnetSetupper01.h
 *
 *  Created on: Mar 5, 2025
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_WALLET_SETUP_TESTNETSETUPPER01_H_
#define BLOCKCHAIN_WALLET_SETUP_TESTNETSETUPPER01_H_

#include <cstdint>

namespace alinous {
class File;
}
using namespace alinous;

namespace codablecash {

class MultizoneTestnet;
class NetworkWallet;
class IDebugSeeder;
class StackTestPortGetter;
class ISystemLogger;

class TestnetSetupper01 {
public:
	static MultizoneTestnet* setup2NodeZone0(NetworkWallet* wallet, uint64_t genesisAmount, IDebugSeeder* seeder,
			StackTestPortGetter* portSel, const File* projectFolder, ISystemLogger* logger);
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_WALLET_SETUP_TESTNETSETUPPER01_H_ */
