/*
 * AbstractTrxMemoryPool.h
 *
 *  Created on: 2023/12/02
 *      Author: iizuka
 */

#ifndef BC_MEMORYPOOL_ABSTRACTTRXMEMORYPOOL_H_
#define BC_MEMORYPOOL_ABSTRACTTRXMEMORYPOOL_H_

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
class UtxoId;
class MempoolUtxoFindResult;

class AbstractTrxMemoryPool {
public:
	static const constexpr wchar_t* NAME_TRX_STORE{L"trxStore"};
	static const constexpr wchar_t* NAME_FEE_RATE_INDEX{L"feeRateIndex"};
	static const constexpr wchar_t* NAME_UTXO_INDEX{L"utxoIndex"};

	explicit AbstractTrxMemoryPool(const File* baseDir);
	virtual ~AbstractTrxMemoryPool();

	bool exists();

	virtual void createBlankPool();

	virtual void open();
	virtual void close() noexcept;

	virtual void putTransaction(const AbstractBlockchainTransaction* trx);
	virtual void removeTransaction(const TransactionId* trxId);

	virtual AbstractBlockchainTransaction* getBlockchainTransaction(const TransactionId* trxId) const;
	virtual TransactionId* getTransactionIdFromUtxoId(const UtxoId* utxoId) const;
	virtual MempoolUtxoFindResult* getUtxo(const UtxoId* utxoId) const;

	virtual BtreeReverseScanner* getScanner() const noexcept;
	virtual BtreeScanner* getSyncScanner() const noexcept;

	virtual int getCount() const;

private:
	void __close(Btree** tree) noexcept;

protected:
	File* baseDir;

	DiskCacheManager* cacheManager;

	Btree* trxStore;
	Btree* feeRateIndex;
	Btree* utxoIndex;
};

} /* namespace codablecash */

#endif /* BC_MEMORYPOOL_ABSTRACTTRXMEMORYPOOL_H_ */
