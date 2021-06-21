/*
 * OidArrayCacheElement.h
 *
 *  Created on: 2021/06/15
 *      Author: iizuka
 */

#ifndef TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYCACHEELEMENT_H_
#define TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYCACHEELEMENT_H_
#include <cstdint>

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class CdbOid;

class OidArrayCacheElement {
public:
	explicit OidArrayCacheElement(int size);
	virtual ~OidArrayCacheElement();

private:
	int size;

	ArrayList<CdbOid> list;

	uint64_t fpos;
	uint64_t nextFpos;
};

} /* namespace codablecash */

#endif /* TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYCACHEELEMENT_H_ */
