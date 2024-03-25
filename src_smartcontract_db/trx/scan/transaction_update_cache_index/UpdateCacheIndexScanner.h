/*
 * UpdateCacheIndexScanner.h
 *
 *  Created on: 2022/01/14
 *      Author: iizuka
 */

#ifndef TRX_SCAN_TRANSACTION_UPDATE_CACHE_INDEX_UPDATECACHEINDEXSCANNER_H_
#define TRX_SCAN_TRANSACTION_UPDATE_CACHE_INDEX_UPDATECACHEINDEXSCANNER_H_

#include "scan_select/scan/RangeChecker.h"

namespace alinous {
class IBtreeScanner;
}
using namespace alinous;

namespace codablecash {

class SingleKeyOidCache;
class CdbRecordKey;
class CdbRecord;
class CdbOid;
class CdbOidValueListCursor;
class OidKeyRecordCache;

class UpdateCacheIndexScanner : public RangeChecker {
public:
	explicit UpdateCacheIndexScanner(SingleKeyOidCache* keyOidCache,
			const CdbRecordKey* begin, bool beginEq, const CdbRecordKey* end, bool endEq, OidKeyRecordCache* recordCache);
	virtual ~UpdateCacheIndexScanner();

	void init();

	bool hasNext();
	const CdbRecord* next();

private:
	SingleKeyOidCache* keyOidCache;
	OidKeyRecordCache* recordCache;

	IBtreeScanner* scanner;
	CdbOidValueListCursor* cursor;
	const CdbOid* nextObj;
};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_UPDATE_CACHE_INDEX_UPDATECACHEINDEXSCANNER_H_ */
