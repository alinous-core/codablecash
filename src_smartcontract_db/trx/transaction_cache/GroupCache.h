/*
 * GroupKeyCache.h
 *
 *  Created on: 2021/05/20
 *      Author: iizuka
 */

#ifndef TRX_TRANSACTION_CACHE_GROUPCACHE_H_
#define TRX_TRANSACTION_CACHE_GROUPCACHE_H_

namespace alinous {
class VirtualMachine;
}  // namespace alinous
using namespace alinous;

namespace codablecash {

class CdbLocalCacheManager;
class CdbRecord;
class ScanResultMetadata;
class GroupByPlanner;
class OidArrayCache;
class GroupRecordCache;
class GroupCacheScanner;

class GroupCache {
public:
	explicit GroupCache(CdbLocalCacheManager* localCacheManager);
	virtual ~GroupCache();

	void init(VirtualMachine* vm);

	void groupRecord(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, GroupByPlanner* groupByPlan);

	GroupCacheScanner* getScanner();

	GroupRecordCache* getCache() const noexcept {
		return cache;
	}

	OidArrayCache* getArrayCache() const noexcept {
		return arrayCache;
	}

private:
	CdbLocalCacheManager* localCacheManager;

	GroupRecordCache* cache;
	OidArrayCache* arrayCache;
};

} /* namespace codablecash */

#endif /* TRX_TRANSACTION_CACHE_GROUPCACHE_H_ */
