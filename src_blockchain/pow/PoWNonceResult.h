/*
 * PoWNonceResult.h
 *
 *  Created on: 2023/03/21
 *      Author: iizuka
 */

#ifndef POW_POWNONCERESULT_H_
#define POW_POWNONCERESULT_H_

#include "pow/PoWNonce.h"

namespace codablecash {

class PoWNonceResult : public PoWNonce {
public:
	explicit PoWNonceResult(const BigInteger* nonce);
	virtual ~PoWNonceResult();
};

} /* namespace codablecash */

#endif /* POW_POWNONCERESULT_H_ */
