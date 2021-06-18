/*
 * OidArrayCache.h
 *
 *  Created on: 2021/06/14
 *      Author: iizuka
 */

#ifndef TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYCACHE_H_
#define TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYCACHE_H_

namespace alinous {
class BlockFileStore;
class DiskCacheManager;
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class CdbOid;

class OidArrayCache {
public:
	static constexpr int INDEX_ELEMENT_SIZE{32};

	OidArrayCache();
	virtual ~OidArrayCache();

	void init(UnicodeString* dir, UnicodeString* name, DiskCacheManager* cacheManager);
	void shutdown();

	void add(int index, CdbOid* oid);

private:
	void initFirstIndexElement();

private:
	BlockFileStore* blockStore;
};

} /* namespace codablecash */

#endif /* TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYCACHE_H_ */
