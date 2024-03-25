/*
 * GroupCacheScanner.h
 *
 *  Created on: 2021/07/12
 *      Author: iizuka
 */

#ifndef TRX_TRANSACTION_CACHE_GROUPCACHESCANNER_H_
#define TRX_TRANSACTION_CACHE_GROUPCACHESCANNER_H_

namespace alinous {
class IBtreeScanner;
}
using namespace alinous;

namespace codablecash {

class GroupCache;
class CdbGroupedRecord;
class OidKeyRecordCache;

class GroupCacheScanner {
public:
	explicit GroupCacheScanner(GroupCache* gcache);
	virtual ~GroupCacheScanner();

	void init();

	bool hasNext();
	const CdbGroupedRecord* next(OidKeyRecordCache* orgCache);
private:
	GroupCache* gcache;

	IBtreeScanner* scanner;
	CdbGroupedRecord* nextObj;
};

} /* namespace codablecash */

#endif /* TRX_TRANSACTION_CACHE_GROUPCACHESCANNER_H_ */
