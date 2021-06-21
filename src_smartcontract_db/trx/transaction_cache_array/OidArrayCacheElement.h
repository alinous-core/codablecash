/*
 * OidArrayCacheElement.h
 *
 *  Created on: 2021/06/15
 *      Author: iizuka
 */

#ifndef TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYCACHEELEMENT_H_
#define TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYCACHEELEMENT_H_
#include <cstdint>

namespace alinous {
class ByteBuffer;
}  // namespace alinous
using namespace alinous;

#include "base/ArrayList.h"

namespace codablecash {

class CdbOid;

class OidArrayCacheElement {
public:
	explicit OidArrayCacheElement(int size);
	virtual ~OidArrayCacheElement();

	int blockSize();
	void toBinary(ByteBuffer* buff);

	uint64_t getFpos() const {
		return fpos;
	}
	void setFpos(uint64_t fpos) {
		this->fpos = fpos;
	}

private:
	int size;

	ArrayList<CdbOid> list;

	uint64_t fpos;
	uint64_t nextFpos;
};

} /* namespace codablecash */

#endif /* TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYCACHEELEMENT_H_ */
