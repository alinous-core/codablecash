/*
 * WalletMemoryPool.h
 *
 *  Created on: Apr 19, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_DATA_WALLETMEMORYPOOL_H_
#define BC_WALLET_NET_DATA_WALLETMEMORYPOOL_H_

namespace alinous {
class File;
class Btree;
class DiskCacheManager;
class BtreeReverseScanner;
class BtreeScanner;
}
using namespace alinous;

namespace codablecash {

class AbstractBlockchainTransaction;
class TransactionId;

class WalletMemoryPool {
public:
	static const constexpr wchar_t* NAME_TRX_STORE{L"trxStore"};

	explicit WalletMemoryPool(File* baseDir);
	virtual ~WalletMemoryPool();

	bool exists();

	void createBlankPool();

	void open();
	void close() noexcept;

	void putTransaction(const AbstractBlockchainTransaction* trx);
	void removeTransaction(const TransactionId* trxId);

	AbstractBlockchainTransaction* getBlockchainTransaction(const TransactionId* trxId) const;
	BtreeScanner* getScanner() const noexcept;

private:
	File* baseDir;

	DiskCacheManager* cacheManager;

	Btree* trxStore;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_DATA_WALLETMEMORYPOOL_H_ */
