/*
 * TransactionTableIndexWrapper.h
 *
 *  Created on: 2022/01/12
 *      Author: iizuka
 */

#ifndef TRX_SCAN_TRANSACTION_UPDATE_CACHE_INDEX_TRANSACTIONTABLEINDEXWRAPPER_H_
#define TRX_SCAN_TRANSACTION_UPDATE_CACHE_INDEX_TRANSACTIONTABLEINDEXWRAPPER_H_

namespace codablecash {

class CdbRecord;
class CdbOid;
class CdbTableIndex;
class CdbStorageManager;
class CdbTable;
class IndexStore;

class CdbLocalCacheManager;
class SingleKeyOidCache;

class UpdateCacheIndexScanner;
class CdbRecordKey;
class OidKeyRecordCache;

class TransactionTableIndexWrapper {
public:
	explicit TransactionTableIndexWrapper(const CdbTable* table, CdbTableIndex* index, CdbStorageManager* storageManager);
	virtual ~TransactionTableIndexWrapper();

	void init(CdbLocalCacheManager* localCacheManager);

	void addRecord(const CdbRecord* record);
	void removeRecord(const CdbOid* oid, const CdbRecord* record);

	UpdateCacheIndexScanner* getUpdateCacheIndexScanner(const CdbRecordKey* begin, bool beginEq, const CdbRecordKey* end, bool endEq, OidKeyRecordCache* recordCache);

	CdbTableIndex* getIndex() const noexcept {
		return index;
	}

private:
	CdbTableIndex* index;
	IndexStore* indexStore;
	SingleKeyOidCache* keyOidCache;
};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_UPDATE_CACHE_INDEX_TRANSACTIONTABLEINDEXWRAPPER_H_ */
