/*
 * OidKeyRecordCacheScanner.h
 *
 *  Created on: 2022/01/09
 *      Author: iizuka
 */

#ifndef TRX_TRANSACTION_CACHE_OIDKEYRECORDCACHESCANNER_H_
#define TRX_TRANSACTION_CACHE_OIDKEYRECORDCACHESCANNER_H_

namespace alinous {
class IBtreeScanner;
}
using namespace alinous;

namespace codablecash {

class CdbRecord;
class OidKeyRecordCache;

class OidKeyRecordCacheScanner {
public:
	explicit OidKeyRecordCacheScanner(OidKeyRecordCache* cache);
	virtual ~OidKeyRecordCacheScanner();

	void init();
	bool hasNext();
	CdbRecord* next();

private:
	OidKeyRecordCache* cache;
	IBtreeScanner* scanner;
};

} /* namespace codablecash */

#endif /* TRX_TRANSACTION_CACHE_OIDKEYRECORDCACHESCANNER_H_ */
