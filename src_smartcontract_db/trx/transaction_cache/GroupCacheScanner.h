/*
 * GroupCacheScanner.h
 *
 *  Created on: 2021/07/12
 *      Author: iizuka
 */

#ifndef TRX_TRANSACTION_CACHE_GROUPCACHESCANNER_H_
#define TRX_TRANSACTION_CACHE_GROUPCACHESCANNER_H_

namespace codablecash {

class GroupCache;

class GroupCacheScanner {
public:
	explicit GroupCacheScanner(GroupCache* gcache);
	virtual ~GroupCacheScanner();

	void init();
private:
	GroupCache* gcache;
};

} /* namespace codablecash */

#endif /* TRX_TRANSACTION_CACHE_GROUPCACHESCANNER_H_ */
