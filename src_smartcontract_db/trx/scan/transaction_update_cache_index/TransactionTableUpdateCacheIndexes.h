/*
 * TransactionTableUpdateCacheIndexes.h
 *
 *  Created on: 2022/01/12
 *      Author: iizuka
 */

#ifndef TRX_SCAN_TRANSACTION_UPDATE_CACHE_INDEX_TRANSACTIONTABLEUPDATECACHEINDEXES_H_
#define TRX_SCAN_TRANSACTION_UPDATE_CACHE_INDEX_TRANSACTIONTABLEUPDATECACHEINDEXES_H_

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class CdbRecord;
class CdbOid;
class CdbTableIndex;
class CdbTable;
class TransactionTableIndexWrapper;
class CdbStorageManager;
class OidKeyRecordCache;
class CdbLocalCacheManager;
class UpdateCacheIndexScanner;
class CdbRecordKey;
class OidKeyRecordCache;

class TransactionTableUpdateCacheIndexes {
public:
	explicit TransactionTableUpdateCacheIndexes(CdbStorageManager* storageManager, OidKeyRecordCache* cache);
	virtual ~TransactionTableUpdateCacheIndexes();

	void addIndex(const CdbTable* table, CdbTableIndex* index, CdbLocalCacheManager* localCacheManager);

	void addRecord(const CdbRecord* record);
	void removeRecord(const CdbOid* oid);

	UpdateCacheIndexScanner* getUpdateCacheIndexScanner(const CdbOid* indexOid, const CdbRecordKey* begin, bool beginEq, const CdbRecordKey* end, bool endEq, OidKeyRecordCache* recordCache);

	TransactionTableIndexWrapper* getIndexWrapper(const CdbOid* indexOid) const noexcept;

private:
	ArrayList<TransactionTableIndexWrapper> list;

	CdbStorageManager* storageManager;
	OidKeyRecordCache* cache;
};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_UPDATE_CACHE_INDEX_TRANSACTIONTABLEUPDATECACHEINDEXES_H_ */
