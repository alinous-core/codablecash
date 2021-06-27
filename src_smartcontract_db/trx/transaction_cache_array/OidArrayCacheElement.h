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
	static OidArrayCacheElement* fromBinary(ByteBuffer* buff, int maxsize);

	uint64_t getFpos() const noexcept {
		return fpos;
	}
	void setFpos(uint64_t fpos) noexcept {
		this->fpos = fpos;
	}
	uint64_t getNextFpos() const noexcept {
		return nextFpos;
	}
	void setNextFpos(uint64_t nextFpos) noexcept {
		this->nextFpos = nextFpos;
	}

	void addOid(const CdbOid* oid);

	bool isFull() const noexcept;
	int size() const noexcept;
	const CdbOid* get(int i) const noexcept;

private:
	int listSize;

	ArrayList<CdbOid> list;

	uint64_t fpos;
	uint64_t nextFpos;
};

} /* namespace codablecash */

#endif /* TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYCACHEELEMENT_H_ */
