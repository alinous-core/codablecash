/*
 * NetworkWalletSync.h
 *
 *  Created on: Apr 18, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_SYNC_NETWORKWALLETSYNC_H_
#define BC_WALLET_NET_SYNC_NETWORKWALLETSYNC_H_

#include <cstdint>

#include "base/ArrayList.h"

namespace alinous{
class File;
class SysMutex;
};
using namespace alinous;

namespace codablecash {

class NetworkWalletData;
class WalletNetworkManager;
class WalletConfig;
class ClientSyncHeaderHeightData;
class ISystemLogger;
class SyncHeaderRepo;
class NodeIdentifier;

class NetworkWalletSync {
public:
	static const constexpr wchar_t* SYNC_DIR{L"synctmp"};

	NetworkWalletSync(uint64_t startHeight, NetworkWalletData* walletData, WalletNetworkManager* networkManager, const WalletConfig* walletConfig, ISystemLogger* logger);
	virtual ~NetworkWalletSync();

	void sync();

	WalletNetworkManager* getWalletNetworkManager() const noexcept {
		return this->networkManager;
	}

	void addHeaderData(const ClientSyncHeaderHeightData* headerData);


private:
	void initHeadersRepo();
	void syncHeader(uint16_t zone, ArrayList<NodeIdentifier>* nodelist);
	void processHeaders();
	void __buildManagementAccounts();

private:
	uint64_t startHeight;

	NetworkWalletData* walletData;
	WalletNetworkManager* networkManager;

	const WalletConfig* walletConfig;
	ISystemLogger* logger;

	File* tmpSyncDir;
	SysMutex* headerDataMutex;
	SyncHeaderRepo* headerDataStore;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_SYNC_NETWORKWALLETSYNC_H_ */
