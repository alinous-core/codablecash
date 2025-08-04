/*
 * NetworkCommandData.h
 *
 *  Created on: Mar 23, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_NETWORKWALLETDATA_H_
#define BC_WALLET_NET_NETWORKWALLETDATA_H_

#include <cstdint>

#include "base/ArrayList.h"

#include "bc_status_cache/IBlockchainStoreProvider.h"


namespace alinous {
class File;
class UnicodeString;
class SysMutex;
class ConcurrentGate;
}
using namespace alinous;

namespace codablecash {

class ISystemLogger;
class HdWallet;
class HdWalletSeed;
class IWalletDataEncoder;
class HeadBlockDetector;
class BlockHeaderStoreManager;
class TransactionTransferData;
class TransactionGroupDataStore;
class WalletMemoryPool;
class BlockHeader;
class AbstractBlockchainTransaction;
class StatusStore;
class CodablecashSystemParam;
class ManagementAccountsCollection;
class ManagementAccount;

class NetworkWalletData : public IBlockchainStoreProvider {
public:
	static const constexpr wchar_t* KEY_ZONE{L"zone"};
	static const constexpr wchar_t* KEY_FINALIZED_HEIGHT{L"finalizedHeight"};
	static const constexpr wchar_t* STATUS_STORE_FILE_NAME{L"status.bin"};

	NetworkWalletData(const File* baseDir, ISystemLogger* logger, const CodablecashSystemParam *config);
	virtual ~NetworkWalletData();

	void close() noexcept;

	void createHdWallet(const HdWalletSeed* seed, uint16_t defaultZone, const IWalletDataEncoder* encoder, int defaultMaxAddress);
	void createBlank();

	HdWallet* getHdWallet() const noexcept {
		return this->hdWallet;
	}

	uint16_t getDefaultZone() const noexcept;

	void addTransactionDataToMempool(const TransactionTransferData *data);

	void addHeader(const BlockHeader* header, ArrayList<AbstractBlockchainTransaction>* trxlist);


	const File* getBaseDir() const noexcept {
		return this->baseDir;
	}

	void __saveStatus();

	virtual BlockHeaderStoreManager* getHeaderManager(uint16_t zone) const noexcept;
	virtual BlockBodyStoreManager* getBlockBodyStoreManager(uint16_t zone) const noexcept;

	void resetManagementAccounts() noexcept;
	void buildManagementAccount(bool buildFinalized);

	void __buildMempoolAccount();
	void __buildUnfinalizedAccount();

	ConcurrentGate* getLock() const noexcept {
		return this->gateLock;
	}

	ManagementAccountsCollection* getManagementAccountsCollection() const noexcept {
		return this->managementAccounts;
	}

private:
	void __buildFinalizedManagementAccount();

	void __buildManagementAccount(ManagementAccount* ma, uint64_t startHeight, uint64_t maxBlockHeight);
	void __buildManagementAccount4Header(ManagementAccount* ma, const BlockHeader* header);


	void __initStatusStore();
	void __doFinalize(const BlockHeader *header);

private:
	File* baseDir;
	ISystemLogger* logger;
	const CodablecashSystemParam *config;

	// wallet (Finalized Data)
	HdWallet* hdWallet;
	uint64_t finalizedHeight;
	uint16_t zone;

	// unfinalized chain
	TransactionGroupDataStore* transactionGroupData;

	BlockHeaderStoreManager* headerManager;
	HeadBlockDetector* detector;

	// memory pool
	WalletMemoryPool* mempool;

	// store
	StatusStore* statusStore;

	// management account
	ManagementAccountsCollection* managementAccounts;

	//
	ConcurrentGate* gateLock;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_NETWORKWALLETDATA_H_ */
