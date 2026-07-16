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
class WalletMemoryPoolScanner;

class WalletMemoryPool {
public:
	static const constexpr wchar_t* NAME_TRX_STORE{L"trxStore"};
	static const constexpr wchar_t* NAME_TRX_ORDER_INDEX{L"trxOrderIndex"};

	explicit WalletMemoryPool(File* baseDir);
	virtual ~WalletMemoryPool();

	bool exists();

	void createBlankPool();

	void open();
	void close() noexcept;

	void putTransaction(const AbstractBlockchainTransaction* trx);
	void removeTransaction(const TransactionId* trxId);

	AbstractBlockchainTransaction* getBlockchainTransaction(const TransactionId* trxId) const;
	bool hasTransaction(const TransactionId* trxId) const;

	WalletMemoryPoolScanner* getScanner() const noexcept;

	BtreeScanner* getBtreeScanner() const noexcept;

private:
	File* baseDir;

	DiskCacheManager* cacheManager;

	Btree* trxStore;
	Btree* trxOrderIndex;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_DATA_WALLETMEMORYPOOL_H_ */
