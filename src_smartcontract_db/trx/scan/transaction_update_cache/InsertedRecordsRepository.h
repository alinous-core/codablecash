/*
 * InsertRecordRepository.h
 *
 *  Created on: 2020/09/28
 *      Author: iizuka
 */

#ifndef TRANSACTION_UPDATE_CACHE_INSERTEDRECORDSREPOSITORY_H_
#define TRANSACTION_UPDATE_CACHE_INSERTEDRECORDSREPOSITORY_H_

namespace alinous {
class IBtreeScanner;
}
using namespace alinous;

namespace codablecash {

class CdbLocalCacheManager;
class CdbStorageManager;
class OidKeyRecordCache;
class CdbRecord;
class CdbOid;
class TransactionTableUpdateCacheIndexes;
class CdbTableIndex;
class CdbTable;
class CdbRecordKey;
class UpdateCacheIndexScanner;

class InsertedRecordsRepository {
public:
	explicit InsertedRecordsRepository(CdbStorageManager* storageManager, CdbLocalCacheManager* cacheManager);
	virtual ~InsertedRecordsRepository();

	void initIndex(const CdbTable* table, CdbTableIndex* index);

	IBtreeScanner* getScanner();

	UpdateCacheIndexScanner* getInsertedCacheIndexScanner(const CdbOid* indexOid, const CdbRecordKey* begin, bool beginEq,
			const CdbRecordKey* end, bool endEq);

	void addRecord(const CdbRecord* record);
	void removeRecord(const CdbOid* oid);
	void updateRecord(const CdbRecord* record);
private:
	OidKeyRecordCache* cache;

	TransactionTableUpdateCacheIndexes* indexes;
	CdbLocalCacheManager* cacheManager;
};

} /* namespace codablecash */

#endif /* TRANSACTION_UPDATE_CACHE_INSERTEDRECORDSREPOSITORY_H_ */
