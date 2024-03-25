/*
 * OidArrayCacheScanner.h
 *
 *  Created on: 2021/06/25
 *      Author: iizuka
 */

#ifndef TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYCACHESCANNER_H_
#define TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYCACHESCANNER_H_

namespace codablecash {

class OidArrayCache;
class OidArrayCacheElement;
class CdbOid;

class OidArrayCacheScanner {
public:
	explicit OidArrayCacheScanner(OidArrayCache* cache);
	virtual ~OidArrayCacheScanner();

	void init(int index);
	bool hasNext();
	const CdbOid* next();

private:
	void resetNextObject() noexcept;
	bool checkCacheElement() noexcept;

private:
	OidArrayCache* cache;
	OidArrayCacheElement* element;
	int cursor;

	const CdbOid* nextOid;
};

} /* namespace codablecash */

#endif /* TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYCACHESCANNER_H_ */
