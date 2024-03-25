/*
 * TransactionContextCache.h
 *
 *  Created on: 2023/04/24
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_CONTEXT_TRANSACTIONCONTEXTCACHE_H_
#define BC_STATUS_CACHE_CONTEXT_TRANSACTIONCONTEXTCACHE_H_

namespace alinous {
class File;
class DiskCacheManager;
class Btree;
class BtreeScanner;
}
using namespace alinous;

namespace codablecash {

class AbstractBlockchainTransaction;
class TransactionId;

class TransactionContextCache {
public:
	static const constexpr wchar_t* NAME_TRX_STORE{L"trxStore"};

	explicit TransactionContextCache(const File* baseDir);
	virtual ~TransactionContextCache();

	void init();

	void open();
	void close();

	void putTransaction(const AbstractBlockchainTransaction* trx);
	bool hasTransaction(const TransactionId* trxId);

	BtreeScanner* getBtreeScanner() const noexcept;

private:
	void __close(Btree** tree) noexcept;

private:
	File* baseDir;
	DiskCacheManager* cacheManager;

	Btree* trxStore;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_CONTEXT_TRANSACTIONCONTEXTCACHE_H_ */
