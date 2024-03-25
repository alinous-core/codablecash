/*
 * UtxoCacheContext.h
 *
 *  Created on: 2023/04/25
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_CONTEXT_UTXOCACHECONTEXT_H_
#define BC_STATUS_CACHE_CONTEXT_UTXOCACHECONTEXT_H_

#include <cstdint>

namespace alinous {
class File;
class DiskCacheManager;
class Btree;
class BtreeScanner;
}
using namespace alinous;

namespace codablecash {

class AbstractUtxo;
class TransactionId;
class UtxoId;
class BlockHeaderId;
class UtxoData;
class AbstractBlockchainTransaction;

class UtxoCacheContext {
public:
	static const constexpr wchar_t* NAME_UTXO_INDEX{L"utxoIndex"};

	explicit UtxoCacheContext(const File* baseDir);
	virtual ~UtxoCacheContext();

	void init();
	void open();
	void close() noexcept;

	UtxoData* getData(const UtxoId* utxoId);
	void putUtxo(const AbstractUtxo *utxo, const TransactionId* trxId, uint64_t height);
	void putUtxo(const UtxoData* data);
	bool removeUtxo(const UtxoId* utxoId, const AbstractBlockchainTransaction *trx);

	BtreeScanner* getBtreeScanner() const noexcept;

private:
	void __close(Btree** tree) noexcept;

private:
	File* baseDir;
	DiskCacheManager* cacheManager;

	Btree* utxoIndex;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_CONTEXT_UTXOCACHECONTEXT_H_ */
