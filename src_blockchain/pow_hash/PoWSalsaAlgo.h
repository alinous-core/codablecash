/*
 * PoWSalsaAlgo.h
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#ifndef POW_HASH_POWSALSAALGO_H_
#define POW_HASH_POWSALSAALGO_H_

#include "pow_hash/AbstractHashAlgorithmElement.h"

namespace codablecash {

class PoWSalsaAlgo : public AbstractHashAlgorithmElement {
public:
	explicit PoWSalsaAlgo();
	virtual ~PoWSalsaAlgo();

	virtual void calulate(const char* in01, const char* in02, const char* in03, char* out) const noexcept;
};

} /* namespace codablecash */

#endif /* POW_HASH_POWSALSAALGO_H_ */
