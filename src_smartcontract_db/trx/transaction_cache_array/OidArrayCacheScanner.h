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

class OidArrayCacheScanner {
public:
	OidArrayCacheScanner(OidArrayCache* cache);
	virtual ~OidArrayCacheScanner();

private:
	OidArrayCache* cache;
};

} /* namespace codablecash */

#endif /* TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYCACHESCANNER_H_ */
