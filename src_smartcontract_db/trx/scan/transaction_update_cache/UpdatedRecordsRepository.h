/*
 * UpdateRecordRepository.h
 *
 *  Created on: 2020/09/28
 *      Author: iizuka
 */

#ifndef TRANSACTION_UPDATE_CACHE_UPDATEDRECORDSREPOSITORY_H_
#define TRANSACTION_UPDATE_CACHE_UPDATEDRECORDSREPOSITORY_H_

namespace alinous {
class IBtreeScanner;
}
using namespace alinous;

namespace codablecash {

class CdbLocalCacheManager;
class CdbStorageManager;
class CdbRecord;
class OidKeyRecordCache;
class CdbOidKey;
class CdbTableIndex;
class CdbTable;
class TransactionTableUpdateCacheIndexes;
class UpdateCacheIndexScanner;
class CdbRecordKey;
class CdbOid;

class UpdatedRecordsRepository {
public:
	explicit UpdatedRecordsRepository(CdbStorageManager* storageManager, CdbLocalCacheManager* cacheManager);
	virtual ~UpdatedRecordsRepository();

	void initIndex(const CdbTable* table, CdbTableIndex* index);

	IBtreeScanner* getScanner();
	void addUpdatedRecord(const CdbRecord* updatedRecord);
	bool isUpdated(const CdbOidKey* recordOidKey);
	const CdbRecord* getUpdatedRecord(const CdbOidKey* recordOidKey);

	UpdateCacheIndexScanner* getUpdatedCacheIndexScanner(const CdbOid* indexOid, const CdbRecordKey* begin, bool beginEq,
			const CdbRecordKey* end, bool endEq);

private:
	OidKeyRecordCache* cache;

	TransactionTableUpdateCacheIndexes* indexes;
	CdbLocalCacheManager* cacheManager;
};

} /* namespace codablecash */

#endif /* TRANSACTION_UPDATE_CACHE_UPDATEDRECORDSREPOSITORY_H_ */
