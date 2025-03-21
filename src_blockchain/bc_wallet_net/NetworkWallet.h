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

class NetworkWallet {
public:
	NetworkWallet(const File* baseDir, ISystemLogger* logger);
	virtual ~NetworkWallet();

	static NetworkWallet* createNewWallet(const File* dir, const UnicodeString* pass, uint16_t zone, int defaultMaxAddress, ISystemLogger* logger);
	static NetworkWallet* resotreWallet(const File* dir, const UnicodeString* pass, uint16_t zone, const HdWalletSeed* rootSeed, int defaultMaxAddress, ISystemLogger* logger);


	HdWalletSeed* getRootSeed(const IWalletDataEncoder* encoder) const;

	void setNetworkSeeder(INetworkSeeder *seeder) noexcept;
	void initNetwork();

	AddressDescriptor* getAddressDescriptor(int accountIndex, int addressIndex) const;

private:
	void doCreateWallet(const IWalletDataEncoder* encoder, const HdWalletSeed* seed, uint16_t zone, int defaultMaxAddress);

private:
	File* baseDir;
	ISystemLogger* logger;

	HdWallet* hdWallet;

	// net
	WalletNetworkManager* networkManager;

	// Staking Seed Manager
	StakingSeedManager* stakingSeedManager;

	// command processor
	NetworkClientCommandProcessor* clientCommandProcessor;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_NETWORKWALLET_H_ */
