/*
 * UnfinalizedChainDataStore.h
 *
 *  Created on: Apr 19, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_DATA_TRANSACTIONGROUPDATASTORE_H_
#define BC_WALLET_NET_DATA_TRANSACTIONGROUPDATASTORE_H_

namespace alinous {
class File;
class Btree;
class DiskCacheManager;
class BtreeReverseScanner;
class BtreeScanner;
}
using namespace alinous;

namespace codablecash {

class HeaderTransactionGroup;
class BlockHeaderId;

/**
 * stores HeaderTransactionGroup
 */
class TransactionGroupDataStore {
public:
	static const constexpr wchar_t* FILE_NAME{L"unfinalized_data"};

	TransactionGroupDataStore(const File* baseDir);
	virtual ~TransactionGroupDataStore();

	void initBlank();

	void open();
	void close() noexcept;

	void add(const BlockHeaderId* headerId, const HeaderTransactionGroup* group);
	HeaderTransactionGroup* getHeaderTransactionGroup(const BlockHeaderId* headerId) const;

private:
	File* baseDir;

	DiskCacheManager* cacheManager;

	Btree* headerGroupStore;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_DATA_TRANSACTIONGROUPDATASTORE_H_ */
