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

class NetworkWallet {
public:
	explicit NetworkWallet(const File* baseDir);
	virtual ~NetworkWallet();

	static NetworkWallet* createNewWallet(const File* dir, const UnicodeString* pass, uint16_t zone);
	static NetworkWallet* resotreWallet(const File* dir, const UnicodeString* pass, uint16_t zone, const HdWalletSeed* rootSeed);


	HdWalletSeed* getRootSeed() const;

	void setNetworkSeeder(INetworkSeeder *seeder) noexcept;

private:
	void doCreateWallet(const IWalletDataEncoder* encoder, const HdWalletSeed* seed, uint16_t zone);

private:
	File* baseDir;

	IWalletDataEncoder* encoder;

	HdWallet* hdWallet;

	// net
	WalletNetworkManager* networkManager;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_NETWORKWALLET_H_ */
