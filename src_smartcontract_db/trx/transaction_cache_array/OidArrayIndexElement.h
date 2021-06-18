/*
 * OidArrayIndexElement.h
 *
 *  Created on: 2021/06/17
 *      Author: iizuka
 */

#ifndef TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYINDEXELEMENT_H_
#define TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYINDEXELEMENT_H_

#include <cstdint>

namespace codablecash {

class OidArrayIndexElement {
public:
	explicit OidArrayIndexElement(int numElements);
	virtual ~OidArrayIndexElement();

private:
	uint64_t fpos;
	int numElements;
	uint64_t* elementsPos;

	uint64_t nextFpos;
};

} /* namespace codablecash */

#endif /* TRX_TRANSACTION_CACHE_ARRAY_OIDARRAYINDEXELEMENT_H_ */
