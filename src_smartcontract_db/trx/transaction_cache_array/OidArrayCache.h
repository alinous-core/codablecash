/*
 * OidArrayCache.h
 *
 *  Created on: 2021/06/14
 *      Author: iizuka
 */

#ifndef TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYCACHE_H_
#define TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYCACHE_H_
#include <cstdint>

namespace alinous {
class BlockFileStore;
class DiskCacheManager;
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class CdbOid;
class OidArrayIndexElement;
class OidArrayCacheElement;
class OidArrayCacheScanner;

class OidArrayCache {
public:
	friend class OidArrayCacheScanner;

	static const constexpr int DEFAULT_INDEX_ELEMENT_SIZE{32};
	static const constexpr int DEFAULT_ARRAY_ELEMENT_SIZE{16};

	OidArrayCache(int indexElementSize = DEFAULT_INDEX_ELEMENT_SIZE, int arrayElementSize = DEFAULT_ARRAY_ELEMENT_SIZE);
	virtual ~OidArrayCache();

	void init(const UnicodeString* dir, const UnicodeString* name, DiskCacheManager* cacheManager);
	void shutdown();

	void add(uint64_t index, const CdbOid* oid);

	OidArrayCacheScanner* getScanner(int index);

private:
	void initFirstIndexElement();
	uint64_t getIndexFpos(int index);

	OidArrayIndexElement* loadOidArrayIndexElement(uint64_t fpos);
	uint64_t createIndexElement();
	void saveIndexElement(OidArrayIndexElement* element);

	OidArrayCacheElement* createOidArrayElement();
	OidArrayCacheElement* loadOidArrayElement(uint64_t fpos);
	void saveOidArrayElement(OidArrayCacheElement* element);

private:
	BlockFileStore* blockStore;

	int indexElementSize;
	int arrayElementSize;

	uint64_t firstIndexFpos;
};

} /* namespace codablecash */

#endif /* TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYCACHE_H_ */
