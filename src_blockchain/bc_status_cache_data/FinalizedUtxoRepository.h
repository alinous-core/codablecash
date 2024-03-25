/*
 * FinalizedUtxoRepository.h
 *
 *  Created on: 2023/05/08
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_DATA_FINALIZEDUTXOREPOSITORY_H_
#define BC_STATUS_CACHE_DATA_FINALIZEDUTXOREPOSITORY_H_

namespace alinous {
class File;
class DiskCacheManager;
class Btree;
}
using namespace alinous;

namespace codablecash {

class UtxoId;
class AbstractUtxo;
class UtxoData;
class TransactionId;
class BlockHeader;

class FinalizedUtxoRepository {
public:
	static const constexpr wchar_t* NAME_UTXO_INDEX{L"utxoIndex"};

	explicit FinalizedUtxoRepository(const File* baseDir);
	virtual ~FinalizedUtxoRepository();

	void initBlank();

	void open();
	void close();

	bool remove(const UtxoId* utxoId);
	void put(const AbstractUtxo* utxo, const TransactionId* trxId, const BlockHeader* header);
	UtxoData* find(const UtxoId* utxoId) const;

private:
	File* baseDir;
	DiskCacheManager* cacheManager;

	Btree* utxoIndex;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_DATA_FINALIZEDUTXOREPOSITORY_H_ */
