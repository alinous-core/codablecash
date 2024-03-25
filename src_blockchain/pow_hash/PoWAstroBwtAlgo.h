/*
 * PoWAstroBwtAlgo.h
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#ifndef POW_HASH_POWASTROBWTALGO_H_
#define POW_HASH_POWASTROBWTALGO_H_

#include "pow_hash/AbstractHashAlgorithmElement.h"

namespace codablecash {

class AstroBwt;

class PoWAstroBwtAlgo : public AbstractHashAlgorithmElement {
public:
	explicit PoWAstroBwtAlgo(const AstroBwt* astroBwt);
	virtual ~PoWAstroBwtAlgo();

	virtual void calulate(const char* in01, const char* in02, const char* in03, char* out) const noexcept;

private:
	const AstroBwt* astroBwt;
};

} /* namespace codablecash */

#endif /* POW_HASH_POWASTROBWTALGO_H_ */
