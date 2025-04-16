/*
 * NetworkWallet.h
 *
 *  Created on: Feb 17, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_NETWORKWALLET_H_
#define BC_WALLET_NET_NETWORKWALLET_H_
#include <cstdint>

namespace alinous {
class File;
class UnicodeString;
}
using namespace alinous;


namespace codablecash {

class HdWallet;
class WalletNetworkManager;
class IWalletDataEncoder;
class HdWalletSeed;
class INetworkSeeder;
class AddressDescriptor;
class ISystemLogger;
class StakingSeedManager;
class NetworkClientCommandProcessor;
class NetworkWalletData;
class CodablecashSystemParam;

class NetworkWallet {
public:
	NetworkWallet(const File* baseDir, ISystemLogger* logger, const CodablecashSystemParam *config);
	virtual ~NetworkWallet();

	void init();

	static NetworkWallet* createNewWallet(const File* dir, const UnicodeString* pass, uint16_t zone, int defaultMaxAddress, ISystemLogger* logger, const CodablecashSystemParam *config);
	static NetworkWallet* resotreWallet(const File* dir, const UnicodeString* pass, uint16_t zone, const HdWalletSeed* rootSeed, int defaultMaxAddress, ISystemLogger* logger, const CodablecashSystemParam *config);

	HdWalletSeed* getRootSeed(const IWalletDataEncoder* encoder) const;

	void initNetwork(INetworkSeeder *seeder, const IWalletDataEncoder* encoder);
	void startNetwork();
	void syncBlockchain();

	void createData();


	AddressDescriptor* getAddressDescriptor(int accountIndex, int addressIndex) const;

	const File* getBaseDir() const noexcept {
		return this->baseDir;
	}

private:
	void setNetworkSeeder(INetworkSeeder *seeder) noexcept;
	void doCreateWallet(const IWalletDataEncoder* encoder, const HdWalletSeed* seed, uint16_t zone, int defaultMaxAddress);

private:
	File* baseDir;
	ISystemLogger* logger;
	CodablecashSystemParam *config;

	// wallet
	NetworkWalletData* walletData;

	// command processor
	NetworkClientCommandProcessor* clientCommandProcessor;

	// net
	WalletNetworkManager* networkManager;

	// Staking Seed Manager
	StakingSeedManager* stakingSeedManager;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_NETWORKWALLET_H_ */
