/*
 * NetworkCommandData.h
 *
 *  Created on: Mar 23, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_NETWORKWALLETDATA_H_
#define BC_WALLET_NET_NETWORKWALLETDATA_H_

#include <cstdint>

namespace alinous {
class File;
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class ISystemLogger;
class HdWallet;
class HdWalletSeed;
class IWalletDataEncoder;
class HeadBlockDetector;
class BlockHeaderStoreManager;

class NetworkWalletData {
public:
	NetworkWalletData(const File* baseDir, ISystemLogger* logger);
	virtual ~NetworkWalletData();

	void createHdWallet(const HdWalletSeed* seed, uint16_t defaultZone, const IWalletDataEncoder* encoder, int defaultMaxAddress);
	void createStores();

	HdWallet* getHdWallet() const noexcept {
		return this->hdWallet;
	}

private:
	File* baseDir;
	ISystemLogger* logger;

	// wallet
	HdWallet* hdWallet;

	BlockHeaderStoreManager* headerManager;
	HeadBlockDetector* detector;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_NETWORKWALLETDATA_H_ */
