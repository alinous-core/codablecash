/*
 * TransactionUpdateCache.h
 *
 *  Created on: 2020/06/13
 *      Author: iizuka
 */

#ifndef TRANSACTION_UPDATE_CACHE_TRANSACTIONUPDATECACHE_H_
#define TRANSACTION_UPDATE_CACHE_TRANSACTIONUPDATECACHE_H_

#include "base/HashMap.h"

namespace alinous {
class File;
}  // namespace alinous
using namespace alinous;

namespace codablecash {

class CdbOid;
class CdbTable;
class InsertLog;
class CdbRecord;
class InsertRecordsCacheCursor;
class TransactionTableUpdateCache;
class CdbLocalCacheManager;
class LocalOidFactory;
class DeletedRecordsOidsCursor;
class UpdatedRecordCursor;
class CdbStorageManager;
class UpdateCacheIndexScanner;
class CdbRecordKey;
class OidWraper;

class TransactionUpdateCache {
public:
	explicit TransactionUpdateCache(CdbStorageManager* storageManager, CdbLocalCacheManager* cacheManager, LocalOidFactory* localOidFacroty);
	virtual ~TransactionUpdateCache();

	void reset() noexcept;

	void updateInsert(InsertLog* cmd, const CdbTable* table);
	void removeInsertedRecord(const CdbOid* oid, const CdbTable* table);
	void updateInsertedRecord(const CdbRecord* updatedRecord, const CdbTable* table) noexcept;
	InsertRecordsCacheCursor* newInsertedRecordsCursor(const CdbTable* table) noexcept;

	void addDeletedRecord(const CdbTable* table, const CdbOid* recordOid);
	bool isDeleted(const CdbTable* table, const CdbOid* recordOid);
	DeletedRecordsOidsCursor* getDeletedRecordsOidsCursor(const CdbTable* table);

	void addUpdatedRecord(const CdbTable* table, const CdbRecord* updatedRecord);
	bool isUpdated(const CdbTable* table, const CdbOid* recordOid);
	const CdbRecord* getUpdatedRecord(const CdbTable* table, const CdbOid* recordOid);
	UpdatedRecordCursor* getUpdatedRecordCursor(const CdbTable* table);

	UpdateCacheIndexScanner* getInsertedCacheIndexScanner(const CdbTable* table, const CdbOid* indexOid, const CdbRecordKey* begin, bool beginEq,
			const CdbRecordKey* end, bool endEq);
	UpdateCacheIndexScanner* getUpdatedCacheIndexScanner(const CdbTable* table, const CdbOid* indexOid, const CdbRecordKey* begin, bool beginEq,
			const CdbRecordKey* end, bool endEq);


private:
	TransactionTableUpdateCache* getTransactionTableUpdateCache(const CdbTable* table) noexcept;

private:
	HashMap<OidWraper, TransactionTableUpdateCache>* tableCashes;

	CdbLocalCacheManager* cacheManager;
	LocalOidFactory* localOidFactory;
	CdbStorageManager* storageManager;

};

} /* namespace codablecash */

#endif /* TRANSACTION_UPDATE_CACHE_TRANSACTIONUPDATECACHE_H_ */
